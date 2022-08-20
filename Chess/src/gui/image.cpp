#include "image.h"

Image::Image(Window* win) {
    this->win = win;
}

void Image::LoadPieces(const char* white_dir, const char* black_dir) {
    // Create basic vector with all the peices
    // Get the loaded peices
    std::map<int, SDL_Texture*> loaded_white_dir = LoadDirectory(white_dir, WHITE);
    std::map<int, SDL_Texture*> loaded_black_dir = LoadDirectory(black_dir, BLACK);
    // Add the black pieces to white
    loaded_white_dir.insert(loaded_black_dir.begin(), loaded_black_dir.end());
    // Set global pieces to all the pieces
    this->g_pieces_images = loaded_white_dir;
    /*for (auto it = loaded_white_dir.begin(); it != loaded_white_dir.end(); ++it) {
        
        
    }*/
}

//SDL_Texture* Image::LoadDirectory(const char* directory) {
std::map<int, SDL_Texture*> Image::LoadDirectory(const char* directory, int side) {
    std::vector<std::string> piece_names = { "Pawn", "Bishop", "Knight", "Rook", "King", "Queen" };
    std::map<int, SDL_Texture*> loaded_images;
    LOG_F(INFO, "Loading %s", directory);
    // Iterate through files in directory
    for (const auto& entry : fs::directory_iterator(directory)) {
        // Get path and filename
        std::string filename = entry.path().stem().filename().string();
        std::string path = entry.path().string();
        // Add them to a map
        loaded_images.insert(
            std::pair<int, SDL_Texture*> (
                // Gets the side and does 'or' operator on the side
                Pieces::c_name_binary.at(filename) | side, LoadImage(this->win->g_renderer, path.c_str())
            )
        );
        // Delete the piece from the vector 
        auto itr = std::find(piece_names.begin(), piece_names.end(), filename);
        if (itr != piece_names.end()) piece_names.erase(itr);
    }
    // Check for missing files and throw error if there are any missing
    if (piece_names.size() != 0) {
        std::string missing_files = "{ ";
        for (std::string file : piece_names) {
            missing_files += file + " ";
        }
        missing_files += "}";
        LOG_F(ERROR, "Missing files %s in %s", missing_files.c_str(), directory);
        this->win->Quit(-1);
    }
    LOG_F(INFO, "Done loading %s", directory);
    return loaded_images;
}

SDL_Texture* Image::LoadImage(SDL_Renderer* renderer, const char* file) {
    // Load an image into SDL
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = IMG_Load(file);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    LOG_F(INFO, "Loaded %s", file);
    return texture;
}
