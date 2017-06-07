#include "frame.h"

Frame::Frame(int id) {
    frameID = id;
    varCount = 0;
    paramCount = 0;
}

int Frame::addparams(std::string id){
    paramCount++;
    Entry e;
    e.id = id;
    e.offset = paramCount*4;
    params.push_back(e);
    return e.offset;
}

int Frame::getparams(std::string id){
    for(auto e : params){
        if(e.id == id)
            return e.offset;
    }
    return 0;
}

int Frame::addLocalVar(std::string id){
    varCount++;
    Entry e;
    e.id = id;
    e.offset = -varCount*4;
    entries.push_back(e);
    return e.offset;
}

int Frame::getLocalVar(std::string id){
    for(int i=varCount-1;i>=0;i--){
        if(entries[i].id==id)
            return entries[i].offset;
    }
    return 0;
}

void Frame::newScope(){
    scopeBegin.push_back(varCount);
}

void Frame::closeScope(){
    int back = scopeBegin[scopeBegin.size()-1];
    for(int i=varCount;i>back;i--){
        entries.pop_back();
    }
    varCount = back;
}

FrameControl::FrameControl() {
    frameCount = 0;
    currentFrame = 0;
	openFrame();
}

void FrameControl::openFrame(){
    frameCount++;   
    Frame f = Frame(frameCount);
    f.staticLinkID = currentFrame;
    currentFrame = frameCount;
    scope.push_back(frameCount);
    Frames.push_back(f);
}

void FrameControl::closeFrame(){
    scope.pop_back();
    currentFrame = scope[scope.size()-1];
}

int FrameControl::addLocalVar(std::string id){
    return Frames[currentFrame-1].addLocalVar(id);
}

int FrameControl::getLocalVar(std::string id){
    return Frames[currentFrame-1].getLocalVar(id);
}

// int FrameControl::addLocalArr(std::string id, const int size){
//     for(int ri = 0; ri < size; ri++)
//         Frames[currentFrame-1].addLocalVar(id + "[" + std::to_string(ri) + "]");
//     return Frames[currentFrame-1].getLocalVar(id + "[" + std::to_string(0) + "]");
// }

// int FrameControl::getLocalArr(std::string id, const int index){
//     return Frames[currentFrame-1].getLocalVar(id + "[" + std::to_string(index) + "]");
// }

// int FrameControl::addLocalRecord(std::string id, std::string subId){
//     return Frames[currentFrame-1].addLocalVar(id + "." + subId);
// }

// int FrameControl::getLocalRecord(std::string id, std::string subId){
//     return Frames[currentFrame-1].getLocalVar(id + "." + subId);
// }

int FrameControl::addparams(std::string id){
    return Frames[currentFrame-1].addparams(id);
}

int FrameControl::getparams(std::string id){
    return Frames[currentFrame-1].getparams(id);
}

void FrameControl::newScope(){
    Frames[currentFrame-1].newScope();
}

void FrameControl::closeScope(){
    Frames[currentFrame-1].closeScope();
}

std::string FrameControl::MEM(std::string id,int cur,std::string substring){
    std::string result;
	if (cur == -1)
		cur = currentFrame;
    int k;
    if((k = Frames[cur-1].getLocalVar(id))!=0){
		result = "MEM(+(CONST(" + std::to_string(k) + ")," + substring + "))";
        return result;
    }
    else if((k=Frames[cur-1].getparams(id))!=0){
        result = "MEM(+(CONST(" + std::to_string(k) + "),"+substring+"))";
        return result;
    }
    else if((k=Frames[cur-1].staticLinkID)!=0){
        result = "MEM(+(CONST(0)," + substring +"))";
        result = MEM(id,k,result);
        return result;
    }
    else{
        result = "Not Found";
        return result;
    }
}

T_mem* FrameControl::MEM(std::string id, int cur, T_exp* subTemp){
    T_mem* root;
    if(subTemp == NULL)
        subTemp = new T_temp("fp");
    if (cur == -1)
        cur = currentFrame;
    int ret;
    if((ret = Frames[cur-1].getLocalVar(id)) != 0){
        root = new T_mem(
            new T_binop(
                T_binOp::T_plus,
                new T_const(ret),
                subTemp
            )
        );
        return root;
    }
    else if((ret = Frames[cur-1].getparams(id)) != 0){
        root = new T_mem(
            new T_binop(
                T_binOp::T_plus,
                new T_const(ret),
                subTemp
            )
        );
        return root;
    }
    else if((ret = Frames[cur-1].staticLinkID) != 0){
        root = new T_mem(
            new T_binop(
                T_binOp::T_plus,
                new T_const(0),
                subTemp
            )
        );
        root = MEM(id, ret, root);
    }
    else{
        return NULL;
    }
}

// T_mem* FrameControl::MEM(std::string id, const int index){
//     return MEM(id + "[" + std::to_string(index), -1, NULL);
// }
// T_mem* FrameControl::MEM(std::string id, const std::string subID){
//     return MEM(id + "." + subID, -1, NULL);
// }

