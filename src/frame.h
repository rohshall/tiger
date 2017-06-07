#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <string>
#include "ir_tree.h"

class Entry {
public:
    int offset;
    std::string id;
};

class Frame {
private:
    int varCount;
    int paramCount;

public:
    int frameID;
    int staticLinkID;
    std::vector<Entry> entries;
	std::vector<Entry> params;
    std::vector<int> scopeBegin;

    Frame(int id);
    int addparams(std::string id);
    int getparams(std::string id);
    int addLocalVar(std::string id);
    int getLocalVar(std::string id);
    void newScope();
    void closeScope();
};

class FrameControl {
private:
    int frameCount;
    int currentFrame;
    std::vector<Frame> Frames;
    std::vector<int> scope;

public:
    FrameControl();
    int addparams(std::string id);
    int getparams(std::string id);
    int addLocalVar(std::string id);
    int getLocalVar(std::string id);
    // int addLocalArr(std::string id, const int size);
    // int getLocalArr(std::string id, const int index);
    // int addLocalRecord(std::string id, std::string subId);
    // int getLocalRecord(std::string id, std::string subId);

    void openFrame();
    void closeFrame();
    void newScope();
    void closeScope();

    std::string MEM(std::string id,int cur = -1, std::string substring = std::string("TEMP(fp)"));
    T_mem* MEM(std::string id, int cur = -1, T_exp* subTemp = NULL);
    // T_mem* MEM(std::string id, const int index = 0);
    // T_mem* MEM(std::string id, const std::string subID);
};

#endif