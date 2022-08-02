#include "renderer.h"


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

Renderer::Renderer(Window* win, Board* board, int render_side) : win(this->win), board(this->board) {
    // I need this cause it doesn't work without it :/
    this->win = win;
    this->board = board;
    // If it should be drawed from white's point of view.
    this->pov_white = render_side == WHITE;
    SDL_SetEventFilter(FilterEvent, this);
    Image image;
    image.win = this->win;
    image.LoadPieces(c_white_dir, c_black_dir);
    this->images = image;
     
    auto values = CalculateDetails(this->win);
    auto size = values.size;
    auto x_offset = values.x_offset;
    auto y_offset = values.y_offset;

	LOG_F(INFO, "Setup renderer.");
}

void Renderer::Render(bool filter_event) {
    if (filter_event) {
        SDL_GetWindowSize(win->g_window, &win->g_width, &win->g_height);
    }
    SDL_RenderClear(this->win->g_renderer);
    DrawBoard(this->pov_white);
    DrawPieces(this->pov_white);
    SDL_RenderPresent(this->win->g_renderer);
}

void Renderer::Update() {
    
}

void Renderer::DrawBoard(bool pov_white) {
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
            if ((x + y) % 2 == 0) {
                if (pov_white) {
                    SDL_SetRenderDrawColor(render, light_square_rgb[0], light_square_rgb[1], light_square_rgb[2], 255);
                }
                else {
                    SDL_SetRenderDrawColor(render, dark_square_rgb[0], dark_square_rgb[1], dark_square_rgb[2], 255);
                }
            }
            else {
                if (pov_white) {
                    SDL_SetRenderDrawColor(render, dark_square_rgb[0], dark_square_rgb[1], dark_square_rgb[2], 255);
                }
                else {
                    SDL_SetRenderDrawColor(render, light_square_rgb[0], light_square_rgb[1], light_square_rgb[2], 255);
                }
            }
            SDL_RenderFillRect(render, &rect);
        }
    }
    SDL_SetRenderDrawColor(render, background_rgb[0], background_rgb[1], background_rgb[2], 255);
}

void Renderer::DrawPieces(bool pov_white) {
    auto board = this->board->GetBoard();
    auto loaded_pieces = this->images.g_pieces_images;
    auto render = this->win->g_renderer;

    auto values = CalculateDetails(this->win);
    auto size = values.size;
    auto x_offset = values.x_offset;
    auto y_offset = values.y_offset;
    int piece_at;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (board[x][y] != BLANK) {
                SDL_Rect rect;
                rect.x = (y * (size + 1)) + x_offset;
                rect.y = (x * (size + 1)) + y_offset;
                rect.w = size + 1;
                rect.h = size + 1;
                piece_at = (pov_white) ? board[x][y] : board[7 - x][7 - y];
                auto piece = loaded_pieces.find(piece_at);
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
    switch (event->type) {
    case SDL_WINDOWEVENT:
        if (event->type == SDL_WINDOWEVENT && (event->window.event == SDL_WINDOWEVENT_RESIZED || event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)) {
            LOG_F(INFO, "Resizing window...\n");
            DrawBoard();
            this->win->ResizeWindow(event->window.data1, event->window.data2);
        }
        break;
    default:
        break;
    }
    return 1;
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
