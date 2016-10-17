/*
* Copyright (C) 2016
* Computer Graphics Group, The Institute of Computer Graphics and Algorithms, TU Wien
* Written by Tobias Klein <tklein@cg.tuwien.ac.at>
* All rights reserved.
*/

#pragma once

#include <glm/glm.hpp>
#include <QString>

class Atom
{
public:

	Atom();
	~Atom();

	float radius;

	int chainId;
	int symbolId;
	int residueId;
	int residueIndex;

	QString helixName = "";
	int helixIndex = -1;

	QString sheetName = "";
	int sheetIndex = -1;

	int nbHelicesPerChain = -1;
	int nbSheetsPerChain = -1;

	QString name;
	QString chain;
	QString symbol;
	QString residueName;

	glm::vec3 position;
	glm::vec3 color;
};