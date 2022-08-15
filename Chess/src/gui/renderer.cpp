#include "renderer.h"
#define INNER_PADDING 10

// Just colors for the board
static int light_square_rgb[3] = { 243, 242, 242 };
static int dark_square_rgb[3] = { 125, 135, 150 };
static int background_rgb[3] = { 100, 100, 100 };

const char* c_white_dir = ".\\resources\\pieces\\white";
const char* c_black_dir = ".\\resources\\pieces\\black";

int FilterEvent(void* userdata, SDL_Event* event);

struct RenderMathValues {
    int shortest;
    int size;
    int x_offset;
    int y_offset;
};

bool CoordEqual(Vector2* point1, Vector2* point2) {
    return point1->x == point2->x && point1->y == point2->y;
}

RenderMathValues CalculateDetails(Window* win) {
    RenderMathValues board;
    // Find shortest side and find size of each square
    board.shortest = (win->g_height > win->g_width) ? win->g_width : win->g_height;
    board.size = (int)(board.shortest/ 8);
    // Offset for resizing
    board.x_offset = (board.shortest == win->g_width) ? 0 : (win->g_width - board.shortest) / 2;
    board.y_offset = (board.shortest == win->g_height) ? 0 : (win->g_height - board.shortest) / 2;
    return board;
}

Renderer::Renderer(Window* win, Board* board) : win(this->win), board(this->board) {
    // I need this cause it doesn't work without it :/
    this->win = win;
    this->board = board;
    // If it should be drawed from white's point of view.
    SDL_SetEventFilter(FilterEvent, this);
    Image image;
    image.win = this->win;
    image.LoadPieces(c_white_dir, c_black_dir);
    this->images = image;

    // Get sizes of images
    for (auto& [piece, texture] : this->images.g_pieces_images) {
        int x = 0;
        int y = 0;
        int p = piece;
        SDL_QueryTexture(texture, NULL, NULL, &x, &y);
        this->sizes.insert(std::pair<int, Vector2>(p, { x, y }));
    }

     
    auto values = CalculateDetails(this->win);
    auto size = values.size;
    auto x_offset = values.x_offset;
    auto y_offset = values.y_offset;

    this->empty_spots = std::vector<std::vector<Empty*>> (
        8,
        std::vector<Empty*>(8)
    );

    // Setup the empty spaces
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            this->empty_spots[x][y] = new Empty(x, y, EMPTY);
        }
    }

	LOG_F(INFO, "Setup renderer.");
}

void Renderer::Render(bool filter_event) {
    if (filter_event) {
        SDL_GetWindowSize(win->g_window, &win->g_width, &win->g_height);
    }
    SDL_RenderClear(this->win->g_renderer);
    DrawBoard();
    DrawPieces();
    //DrawPossibleMoves(this->pov_white);
    SDL_RenderPresent(this->win->g_renderer);
}

void Renderer::DrawBoard() {
    //LOG_F(ERROR, "RAAHHHHHH");
    // Get the renderer and clear it
    SDL_Renderer* render = win->g_renderer;
    auto values = CalculateDetails(this->win);
    auto size = values.size;
    auto x_offset = values.x_offset;
    auto y_offset = values.y_offset;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            SDL_Rect rect;
            rect.x = (x * (size + 1)) + x_offset;
            rect.y = (y * (size + 1)) + y_offset;
            rect.w = size + 1;
            rect.h = size + 1;
            this->empty_spots[x][y]->g_box = rect;
            if (this->board->g_board_colors[x][y] == WHITE) {
                SDL_SetRenderDrawColor(render, light_square_rgb[0], light_square_rgb[1], light_square_rgb[2], 255);
            } else {
                SDL_SetRenderDrawColor(render, dark_square_rgb[0], dark_square_rgb[1], dark_square_rgb[2], 255);

            }
            SDL_RenderFillRect(render, &rect);
        }
    }
    SDL_SetRenderDrawColor(render, background_rgb[0], background_rgb[1], background_rgb[2], 255);
}

void Renderer::DrawPieces() {
    auto board = this->board->GetBoard();
    auto loaded_pieces = this->images.g_pieces_images;
    auto render = this->win->g_renderer;

    auto values = CalculateDetails(this->win);
    auto size = values.size;
    auto x_offset = values.x_offset;
    auto y_offset = values.y_offset;

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (board[x][y]->g_piece != BLANK) {
                // I have to do this because somewhere in my code, the coordinates get updated to the wrong ones.
                // board[x][y]->g_coord = { x, y };
                // Find the texture of the current piece
                auto piece = loaded_pieces.find(board[x][y]->g_piece);
                auto image_size = this->sizes.find(board[x][y]->g_piece);
                SDL_Rect rect;
                // Correct scaling to maintain aspect ratio of images
                if (image_size->second.x > image_size->second.y) {
                    float ratio = (float)image_size->second.x / (float)image_size->second.y;
                    rect.w = size - 2 * INNER_PADDING;
                    rect.h = rect.w / ratio;

                } else {
                    float ratio = (float)image_size->second.y / (float)image_size->second.x;
                    rect.h = size - 2 * INNER_PADDING;
                    rect.w = rect.h / ratio;
                }
                // Set the size of the images
                // Set the offset of the piece (not centered, but at the top left of the square)
                if (this->selected_piece &&
                    board[x][y]->g_coord.x == this->selected_piece->g_coord.x &&
                    board[x][y]->g_coord.y == this->selected_piece->g_coord.y) {
                    // Move selected piece to a coordinate and center it on the mouse
                    SDL_Point clickOffset;
                    rect.x = this->mouse_pos.x - (rect.w)/2;
                    rect.y = this->mouse_pos.y - (rect.h)/2;
                } else {
                    // Swap so we render up down instead of left right
                    rect.x = (y * (size + 1)) + x_offset;
                    rect.y = (x * (size + 1)) + y_offset;
                    // Center the piece in the box
                    rect.x += (size - rect.w) / 2;
                    rect.y += (size - rect.h) / 2;
                }
                this->board->g_game_board[x][y]->g_box = rect;
                if (piece == loaded_pieces.end()) {
                    LOG_F(ERROR, "Error loading piece |0x%0.4x|", piece);
                }
                else {
                    SDL_RenderCopy(render, piece->second, NULL, &rect);
                }
            }
        }
    }
}

int Renderer::HandleInput(SDL_Event* event) {
    // Window event handling
    switch (event->type) {
    case SDL_WINDOWEVENT:
        if (event->type == SDL_WINDOWEVENT && (event->window.event == SDL_WINDOWEVENT_RESIZED || event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)) {
            LOG_F(INFO, "Resizing window...");
            DrawBoard();
            this->win->ResizeWindow(event->window.data1, event->window.data2);
        }
        break;
    case SDL_MOUSEMOTION:
        this->mouse_pos = { event->motion.x, event->motion.y };
    case SDL_MOUSEBUTTONDOWN:
        if (!this->mouse_down && event->button.button == SDL_BUTTON_LEFT) {
            this->mouse_down = true;
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    if (this->board->g_game_board[x][y] == BLANK)
                        break;
                    Piece* piece = this->board->g_game_board[x][y];
                    if (SDL_PointInRect(&this->mouse_pos, &piece->g_box)) {
                        this->selected_piece = piece;
                        break;
                    }
                }
            }
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (this->mouse_down && event->button.button == SDL_BUTTON_LEFT) {
            MouseUp(event);
            //this->board->PrintBoard();
            this->mouse_down = false;
            this->selected_piece = NULL;
        }
        break;
    default:
        break;
    }
    return 1;
}

void Renderer::MouseUp(SDL_Event* event) {
    if (!this->mouse_down || this->selected_piece == NULL) {
        return;
    }
    Piece* dropped_box = NULL;
    for (int x = 0; x < 8; x++) {
        if (dropped_box != NULL) {
            break;
        }
        for (int y = 0; y < 8; y++) {
            /* Make sure that the piece isn't the same as the selected piece that the piece was in */
            //if (CoordEqual(&this->selected_piece->g_coord, &this->board->g_game_board[x][y]->g_coord) ||
            //    /* Check for empty boxes as well */
            //    CoordEqual(&this->selected_piece->g_coord, &this->empty_spots[x][y]->g_coord)) {
            //    continue;
            //}
            if (/* Check if player dropped it on a piece */
                //SDL_PointInRect(&this->mouse_pos, &this->board->g_game_board[x][y]->g_box) ||
                /* Check if player dropped it on an empty box */
                SDL_PointInRect(&this->mouse_pos, &this->empty_spots[x][y]->g_box)) {
                dropped_box = this->empty_spots[y][x];
                break;
            }
        }
    }
    if (dropped_box == NULL) {
        return;
    }
    Vector2 start = this->selected_piece->g_coord;
    Vector2 end = dropped_box->g_coord;
    // Honestly, I'm not sure why this is the way it is, but it works and I'm too lazy to make a proper implementation
    this->board->Move(start.x, start.y, end.x, end.y);
}

int FilterEvent(void* userdata, SDL_Event* event) {
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        // convert userdata pointer to yours and trigger your own draw function
        // this is called very often now
        // IMPORTANT: Might be called from a different thread, see SDL_SetEventFilter docs	
        ((Renderer*)userdata)->Render(true);
        //return 0 if you don't want to handle this event twice
        return 0;
    }
    //important to allow all events, or your SDL_PollEvent doesn't get any event
    return 1;
}
