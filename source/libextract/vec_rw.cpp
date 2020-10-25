#include "vec_rw.h"

struct Vec_RWops {
	SDL_RWops base;
	std::vector<uint8_t> vec;
};

static int delete_vec_close(SDL_RWops* rw) {
	if (rw) {
		Vec_RWops* rw2 = (Vec_RWops*) rw;
		rw2->~Vec_RWops();
		SDL_FreeRW(rw);
	}
	return 0;
}

SDL_RWops* create_vec_rwops(std::vector<uint8_t> buffer) {
	// Use a modified RWFromConstMem so the JS audio code knows how to slurp it
	SDL_RWops* rw_base = SDL_RWFromConstMem(buffer.data(), buffer.size());
	Vec_RWops* rw = (Vec_RWops*) SDL_malloc(sizeof(Vec_RWops));
	memcpy(rw, rw_base, sizeof(SDL_RWops));
	SDL_FreeRW(rw_base);
	new(&rw->vec) std::vector<uint8_t>(std::move(buffer));
	rw->base.close = delete_vec_close;
	return &rw->base;
}
