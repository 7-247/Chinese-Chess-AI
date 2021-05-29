#pragma once
#include <iostream>
#include <map>
#include<algorithm>
#include<string>
#include"Position.h"
struct BookStruct {
	std::string now,Mirrornow;
	bool updown,leftright;
	int N[32],MirrorN[32],result;
	BookStruct();
	int find(PositionStruct&);
	void getNow(PositionStruct&);
};
extern const std::string ss[];
extern const int tt[];