#include "potentialMap.h"
#include "distanceMap.h"
#include "brushfireMap.h"

#include <cmath>
#include <iostream>
#include <string>
#include <stdio.h>

double** potentialMap::generate(distanceMap& dm, brushfireMap& bm) {

	width_ = dm.width();
	height_ = dm.height();

	map_ = new double*[height_];

	for (int i = 0; i < height_; i++) {
		map_[i] = new double[width_];
	}

	for (int i = 0; i < height_; i++) {
		for (int j = 0; j < width_; j++) {
			//Potential Function as sum of the attractive potential(distance) and the repulsive
			//potential(the lower brushfire values)

			map_[i][j] = 20 * (dm.map()[i][j])
					+ 1000000 * (pow(1 / bm.map()[i][j], 3));

		}
	}

	return map_;
}

vector<pos*> potentialMap::findPath(int i, int j) {
	double min_potential = map_[i][j];
//	double max_potential = map_[i][j];
	int tmp_k = i, tmp_l = j;
//	int tmpmax_k = i, tmpmax_l = j;

	path_.push_back(new pos(tmp_k, tmp_l));

	while (min_potential != 0) {
		for (int k = i - 1; k < i - 1 + 3; k++) {
			if (k >= 0 && k < height_)
				for (int l = j - 1; l < j - 1 + 3; l++) {
					//printf("L=%d\n",l);

					if (l >= 0 && l < width_ && !(k == i && l == j)) {
						if (map_[k][l] < map_[tmp_k][tmp_l]) {
							tmp_k = k;
							tmp_l = l;
						}
					}
				}
		}
		path_.push_back(new pos(tmp_k, tmp_l));

		if (min_potential == map_[tmp_k][tmp_l] && tmp_k == i && tmp_l == j) {
			printf("can't move further %d %d\n", tmp_k, tmp_l);
			break;
		} else {
			i = tmp_k;
			j = tmp_l;
			min_potential = map_[tmp_k][tmp_l];
		}
	}


//	while (max_potential != 0) {
//		for (int k = i - 1; k < i - 1 + 3; k++) {
//			if (k >= 0 && k < height_)
//				for (int l = j - 1; l < j - 1 + 3; l++) {
//					//printf("L=%d\n",l);
//
//					if (l >= 0 && l < width_ && !(k == i && l == j)) {
//						if (map_[k][l] > map_[tmpmax_k][tmpmax_l]) {
//							tmpmax_k = k;
//							tmpmax_l = l;
//						}
//					}
//				}
//		}
//
//		if (max_potential == map_[tmpmax_k][tmpmax_l] && tmpmax_k == i && tmpmax_l == j) {
//					printf("MAXX =  %d %d\n", tmpmax_k, tmpmax_l);
//					break;
//				} else {
//					i = tmpmax_k;
//					j = tmpmax_l;
//					max_potential = map_[tmp_k][tmp_l];
//				}
//	}



	for (int k = 0; k < height_; k++) {
		for (int j = 0; j < width_; j++) {
			if (j % 5 == 0)
				printf("Potencial (%d ,%d) = %f\n", k, j, map_[k][j]);
			else
				printf("Potencial (%d ,%d) = %f  ", k, j, map_[k][j]);
		}
	}
	printf("\nPotencial minimo: (%d, %d) = %f \n", tmp_k, tmp_l,
			map_[tmp_k][tmp_l]);
//	printf("\nPotencial maximo: (%d, %d) = %f \n", tmp_k, tmp_l,
//				map_[tmp_k][tmp_l]);
	printf("can't move further %d %d\n", tmp_k, tmp_l);
	printf("Potencial en ( 360, 100) = %f \n", map_[360][100]);

	//printf("Potential maximoooooooo = %f\n", max);

	return path_;
}

double** potentialMap::map() {
	return map_;
}

vector<pos*> potentialMap::path() {
	return path_;
}

int potentialMap::width() {
	return width_;
}

int potentialMap::height() {
	return height_;
}

int potentialMap::pathLen() {
	return pathLen_;
}

potentialMap::~potentialMap() {
	for (int i = 0; i < height_; i++)
		delete map_[i];

	delete[] map_;
}

