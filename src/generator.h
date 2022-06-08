#pragma once
#include <string>
#include <cstring>

const char* valid_chars = "0123456789abcdefghijklmnopqrstuvwxyz";


inline std::string generate_new_image_url() {
	srand(time(NULL));
	std::string url = "https://prnt.sc/";
	for(int i = 0; i < 6; i++) {
		url += valid_chars[rand() % strlen(valid_chars)];
	}
	return url;
}