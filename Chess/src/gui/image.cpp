#include "image.h"

Image::Image(Window* win) {
    this->win = win;
}

void Image::LoadPieces(json settings) {
    // Create basic vector with all the peices
    // Get the loaded peices
    std::cout << settings << std::endl;
    this->settings = settings;
	std::string white_dir = (std::string)settings["base-directory"] + (std::string)settings["white-directory"];
	std::string black_dir = (std::string)settings["base-directory"] + (std::string)settings["black-directory"];
    std::map<int, SDL_Texture*> loaded_white_dir = LoadDirectory(white_dir.c_str(), WHITE);
    std::map<int, SDL_Texture*> loaded_black_dir = LoadDirectory(black_dir.c_str(), BLACK);
    // Add the black pieces to white
    loaded_white_dir.insert(loaded_black_dir.begin(), loaded_black_dir.end());
    // Set global pieces to all the pieces
    this->g_pieces_images = loaded_white_dir;
}

//SDL_Texture* Image::LoadDirectory(const char* directory) {
std::map<int, SDL_Texture*> Image::LoadDirectory(std::string directory, int side) {
    std::vector<std::string> piece_names = { "Pawn", "Bishop", "Knight", "Rook", "King", "Queen" };
    // Gets the file names for each file
    std::map<std::string, std::string> file_names;
    // Get the prefix for each side
    std::string prefix = (side == WHITE) ? this->settings["white-prefix"] : this->settings["black-prefix"];
    for (std::string name : piece_names) {
        file_names.insert({ name, directory + prefix + (std::string)this->settings["image-names"][name] +
                                (std::string)this->settings["extension"] });
    }
    std::map<int, SDL_Texture*> loaded_images;
    LOG_F(INFO, "Loading %s", directory.c_str());
    // Iterate through files in directory
    for (auto &[ name, path ] : file_names) {
        // Get path and filename
        fs::path file(path);
        fs::path filename = file.filename();
        // Add them to a map
        SDL_Texture* loaded_image;
        if (filename.extension() == "svg") {
            loaded_image = LoadSVG(this->win->g_renderer, path.c_str());
        }
        else {
            loaded_image = LoadImage(this->win->g_renderer, path.c_str());
        }
        loaded_images.insert(
            std::pair<int, SDL_Texture*> (
                // Gets the side and does 'or' operator on the side
                Pieces::c_name_binary.at(name) | side, loaded_image
            )
        );
        // Delete the piece from the vector 
        auto itr = std::find(piece_names.begin(), piece_names.end(), name);
        if (itr != piece_names.end()) piece_names.erase(itr);
    }
    // Check for missing files and throw error if there are any missing
    if (piece_names.size() != 0) {
        std::string missing_files = "{ ";
        for (std::string file : piece_names) {
            missing_files += file + " ";
        }
        missing_files += "}";
        LOG_F(ERROR, "Missing files %s in %s", missing_files.c_str(), directory.c_str());
        this->win->Quit(-1);
    }
    LOG_F(INFO, "Done loading %s", directory.c_str());
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

SDL_Texture* Image::LoadSVG(SDL_Renderer* renderer, const char* file) {
    SDL_RWops *rw = SDL_RWFromFile(file, "rb");
    SDL_Surface *surface = IMG_LoadSVG_RW(rw);
    SDL_Texture* texture = NULL;
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    LOG_F(INFO, "Loaded %s", file);
    return texture;
}
