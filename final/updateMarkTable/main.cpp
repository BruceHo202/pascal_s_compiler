/*
占的空间
层数
每层编号

*/
#include "mergeAssist.h"
using namespace std;

FILE * inptr = fopen ("/Users/mac/Desktop/resource/MarkTable.txt", "r"); //原符号表
FILE * mergeptr = fopen("/Users/mac/Desktop/resource/MergedMarkTable.txt", "w+"); //合并后的符号表
FILE * updateptr = fopen("/Users/mac/Desktop/resource/UpdatedMarkTable.txt", "w+"); //原符号表,const、type替换后，加上指向
FILE * stdoutptr = fopen("/Users/mac/Desktop/resource/Fstdout.txt", "w+"); //词法分析后的程序

string merge_dir = "/Users/mac/Desktop/MergedMarkTable.dat";
string update_dir = "/Users/mac/Desktop/UpdatedMarkTable.dat";

void Inititalize(){ //初始化
    memset(oMark, 0, sizeof(oMark));
    memset(mMark, 0, sizeof(mMark));
    memset(uMark, 0, sizeof(uMark));
}

void Store(char * ss, int chnum){ //已经读入一行，将该行存储到oMark中
    char mark[30], value[30];
    int markPos, valuePos;

    ++oMarkNum;
    memset(mark, 0, sizeof(mark)); markPos = 0;
    memset(value, 0, sizeof(value)); valuePos = 0;
    int pos = 2;
    while(pos < chnum && ss[pos] != ','){
        mark[markPos++] = ss[pos];
        pos++;
    }
    pos++;
    while(pos < chnum){
        value[valuePos++] = ss[pos];
        pos++;
    }

    memcpy(oMark[oMarkNum].mark, mark, sizeof(mark));
    memcpy(oMark[oMarkNum].value, value, sizeof(value));
    
    // fprintf(updateptr,"(%s,%s)\n",oMark[oMarkNum].mark,oMark[oMarkNum].value);
}

void ReadMarkTable(){ //从文件读入符号表
    char ss[55]; int chnum = 0;
    while(true){
        char ch = fgetc(inptr);
        if(ch == EOF) break;
        if(ch == '\n') {
            Store(ss, chnum); //存储到oMark中
            chnum = 0;
            continue;
        }
        ss[++chnum] = ch;
    }
}


int GetType(char *ss, char * s){ //类型
    // printf("%s\n", s);
    if(strcmp(s, "program") == 0) return programType;
    else if(strcmp(s, "function") == 0) return functionType;
    else if(strcmp(s, "procedure") == 0) return procedureType;
    else if(strcmp(s, "record") == 0) return recordType;
    else if(strcmp(s, "const") == 0) return constType;
    else if(strcmp(s, "type") == 0) return typeType;
    else if(strcmp(s, "var") == 0) return varType;
    else if(strcmp(ss, "id") == 0) return idType;
    else if(strcmp(s, "begin") == 0) return beginType;
    else if(strcmp(s, "end") == 0) return endType;
    else if(strcmp(s, "case") == 0) return caseType;
    else return -1;
}

void CodeLayer(){ //划分作用域
    stack<WorkSpace> sta;
    while(!sta.empty()) sta.pop();

    int dep = 0;
    for(int i = 1; i <= oMarkNum; i++){
        int tp = GetType(oMark[i].mark, oMark[i].value);
        if((tp >= 1 && tp <= 4) ) { // program,function,procedure,record
            sta.push(WorkSpace(i, -1, ++dep, tp, 0, 0));
            // printf("input1:%d\n", i);
        }
        else if(tp == beginType || tp == caseType){
            sta.push(WorkSpace(i, -2, dep, tp, 0, 0));
            // printf("input2:%d\n",i);
        }
        else if(tp == endType){
            if(sta.top().End == -2){//碰见begin或case
                sta.pop();
                // printf("output0:%d\n", i);
                // continue;
                if(sta.top().End == -2) continue;
                else {
                    dep--;
                    workSpace[++workSpaceNum] = sta.top();
                    workSpace[workSpaceNum].End = i + 1;
                    if(workSpace[workSpaceNum].kind == recordType) workSpace[workSpaceNum].End--;
                    workSpace[workSpaceNum].id = workSpaceNum;
                    sta.pop();
                    // printf("output1:%d\n", i);
                    // printf("<<%d %d>>\n", workSpace[workSpaceNum].Begin, workSpace[workSpaceNum].End);
                }
            }
            else {
                
                // printf("output2:%d\n", i);
                dep--;
                workSpace[++workSpaceNum] = sta.top();
                workSpace[workSpaceNum].End = i + 1;
                if(workSpace[workSpaceNum].kind == recordType) workSpace[workSpaceNum].End--;
                workSpace[workSpaceNum].id = workSpaceNum;
                // printf("%d %d\n", workSpace[workSpaceNum].Begin, workSpace[workSpaceNum].End, )
                    // printf("<<%d %d>>\n", workSpace[workSpaceNum].Begin, workSpace[workSpaceNum].End);
                
                sta.pop();
            }
        }
    }
    //head和body划分
    int nowNum = workSpaceNum;
    for(int i = 1; i <= nowNum; i++){
        WorkSpace ws = workSpace[i];
        if(ws.kind == recordType) {
            workSpace[i].depth++;
            continue;
        }
        int fr = ws.Begin + 1, to = ws.End;
        while(fr <= to){
            int tp = GetType(oMark[fr].mark, oMark[fr].value);
            if((tp >= 1 && tp <= 9) && tp != idType && tp != recordType){
                workSpace[++workSpaceNum] = workSpace[i];
                workSpace[workSpaceNum].Begin = fr;
                workSpace[workSpaceNum].depth++;
                workSpace[workSpaceNum].id = workSpaceNum;
                break;
            }
            fr++;
        }
    }
    sort(workSpace + 1, workSpace + workSpaceNum + 1);

    for(int i = 1; i <= workSpaceNum; i++){ //找父作用域
        int MinNum = oMarkNum + 10;
        for(int j = 1; j <= workSpaceNum; j++)if(i != j){ //遍历所有找父作用域
            WorkSpace ws1 = workSpace[i];
            WorkSpace ws2 = workSpace[j];
            if(ws2.Begin <= ws1.Begin && ws2.End >= ws1.End && ws2.End - ws2.Begin < MinNum && ws2.depth >= ws1.depth - 1){
                MinNum = ws2.End - ws2.Begin;
                workSpace[i].root = j;
            }
        }
    }

    for(int i = 1; i <= workSpaceNum; i++){ //每个符号赋工作域
        WorkSpace ws = workSpace[i];
        for(int j = ws.Begin; j <= ws.End; j++){
            WorkSpace ws2 = workSpace[Belong[j]];
            if(ws2.depth < ws.depth || (ws2.depth == ws.depth && ws.Begin >= ws2.Begin && ws.End <= ws2.End)){
                Belong[j] = i;
            }
        }
    }
    // for(int i = 1; i <= oMarkNum; i++){
    //     printf("%d %d %d\n",i, Belong[i], workSpace[Belong[i]].depth);
    // }
    // for(int i = 1; i <= workSpaceNum; i++){
    //     WorkSpace ws2 = workSpace[workSpace[i].root];
    //     printf("%d (%d %d) depth = %d (%d %d)\n", i, workSpace[i].Begin, workSpace[i].End, workSpace[i].depth, ws2.Begin, ws2.End);
    // }
}

int tpNow = 0;

int FindPos(int x ,int addV){ //第x行是个id，找这个id第一次声明的位置，如果没有，返回-1
    int wsId = Belong[x];
    int en = min(x, workSpace[wsId].End);
    int depth = workSpace[wsId].depth + addV;
    // if(x == 43) printf("depth = %d\n", depth);
    while(depth >= 1){
        WorkSpace ws = workSpace[wsId];
        // printf("depth = %d %d\n", ws.depth, x);
        for(int i = ws.Begin; i <= en; i++){
            // if(x == 43) printf("<<%d %d %d %s %s %d %d>>\n", addV, ws.Begin, ws.End, oMark[i].mark, oMark[i].value, depth, ws.depth);
            if(workSpace[Belong[i]].depth != depth) continue;
            // if(x == 49) printf("<<%d %d %d %s %s %d %d>>\n", addV, ws.Begin, ws.End, oMark[i].mark, oMark[i].value, depth, ws.depth);
            
            int tp = GetType(oMark[i].mark, oMark[i].value);
            if(tp == recordType) tp = varType;
            if((tp >= 1 && tp <= 3) || (tp >= 5 && tp <= 7)){
                tpNow = tp;
                int id = i + 1;
                int tag = 0; //[]
                while(id <= en){
                    // if(x == 4 && ws.Begin == 1) printf("id = %d\n", id);
                    if(strcmp(oMark[id].value, "[") == 0) tag++;
                    if(strcmp(oMark[id].value, "]") == 0) tag--;
                    if(workSpace[Belong[id]].depth != depth || tag) {
                        id++;
                        continue;
                    }
                    int tpnow = GetType(oMark[id].mark, oMark[id].value);
                    // printf("deeper: %d %d %d %s %s\n", id, tpnow, oMark[id].value, oMark[x].value);
                    if((tpnow >= 1 && tpnow <= 9) && tpnow != idType && tpnow != recordType){
                        break;
                    }
                    if(tpnow == idType && strcmp(oMark[id].value, oMark[x].value) == 0 && strcmp(oMark[id - 1].value, ":") != 0) {
                        return id;
                    }
                    id++;
                }
                i = id - 1;
            }
        }
        if(workSpace[workSpace[wsId].root].depth != workSpace[wsId].depth) depth--;
        if(workSpace[wsId].root) wsId = workSpace[wsId].root; //当前作用域没有找到，找父作用域
    }
    return -1;
}

bool noDefId(char *s){//判断是不是不需要定义的id，如write
    if(strcmp(s, "write") == 0) return true;
    if(strcmp(s, "writeln") == 0) return true;
    if(strcmp(s, "read") == 0) return true;
    if(strcmp(s, "readln") == 0) return true;
    return false;
}

void ReplaceConstTypeAndMerge(){ //const、type替换，符号表合并
    memcpy(uMark, oMark, sizeof(oMark)); //原符号表复制给更新的符号表
    uMarkNum = oMarkNum;

    int mpConst[55000] = {0}; //const a = b,mpConst从a对应到b
    int mpType[55000] = {0}; //type a = b, mpType从a到b

    int NewToOld[55000]; //合并后的id符号表和pos的映射

    int deviation[55000] = {0}; //记录偏移，deviation[i]表示第i个区域出现了多少个
    
    int offset1 = 0, offset2 = 0;
    for(int i = 1; i <= oMarkNum; i++){
        // printf("i = %d ", i);
        // if(strcmp(oMark[i].value, "end") == 0) {
        //     if(offset2 >= 1) offset2--;
        // }
        // puts("?");
        if(strcmp(oMark[i].mark, "id") != 0) continue;
        
        if(noDefId(oMark[i].value)){ ////判断是不是不需要定义的id，如write

            int flag = false;
            for(int j = 1; j <= mMarkNum; j++){
                if(strcmp(mMark[j].value, oMark[i].value) == 0) {
                    flag = true;
                    uMark[i].pos = mMark[j].pos;
                    break;
                }
            }
            if(flag == false){
                mMarkNum++;
                Z_marktable &mM = mMark[mMarkNum], oM = oMark[i];
                memcpy(mM.mark, oM.mark, sizeof(oM.mark));
                memcpy(mM.value, oM.value, sizeof(oM.value));
                mM.pos = i;

                uMark[i].pos = i;
            }
            continue;
        }
        tpNow = 0; //找到该变量的类型
        
        if(i >= 1 && strcmp(oMark[i - 1].value, ".") == 0) offset1++;
        else offset1 = 0;
        // if(i >= 1 && strcmp(oMark[i - 1].value, "record") == 0) offset2++;


        // if(i == 49) printf("%d\n")

        // if(i == 129) printf("%d\n", offset);
        int pos = FindPos(i, offset1);
        // printf("%d %d\n")
        // printf("%d %s %d\n", i, oMark[i].value, pos);
        if(pos == -1){
            printf("error in MarkTable line %d: find undefined variable!!!\n", i);
            exit(0);
        }
        // if(i == 49) printf("pos = %d %d\n", pos, tpNow);
        if(tpNow == functionType || tpNow == programType || tpNow == procedureType){
            int tp2 = GetType(oMark[pos - 1].mark, oMark[pos - 1].value);
            if(tp2 != functionType && tp2 != programType && tp2 != procedureType) tpNow = varType;
        }
        
        // if(pos == 49) printf("??pos = %d %d\n", pos, tpNow);
        if(pos == i){ //是新的定义，加入到mergedMark符号表中
            mMarkNum++;
            Z_marktable &mM = mMark[mMarkNum], oM = oMark[i];
            memcpy(mM.mark, oM.mark, sizeof(oM.mark));
            memcpy(mM.value, oM.value, sizeof(oM.value));
            mM.pos = pos;
            mM.depth = workSpace[Belong[i]].depth;
            mM.kind = tpNow;

            //更新区域id数量并跟新对应id的偏移
            // deviation[Belong[pos]]++;
            // mM.deviation = deviation[Belong[pos]];

            //更新belong
            if(tpNow == varType){
                int id = i;
                while(id <= oMarkNum){
                    if(strcmp(oMark[id].value, ":") == 0) {
                        mM.Belong = id + 1;
                        break;
                    }
                    id++;
                }
            }

            NewToOld[pos] = mMarkNum;
            // printf("i = %d\n", i);
            if(tpNow == constType){
                int id = i;
                while(id <= oMarkNum){
                    if(strcmp(oMark[id].value, "=") == 0){
                        id++;
                        mpConst[mMarkNum] = id;
                        break;
                    }
                    id++;
                }
            }
            if(tpNow == typeType){
                int id = i;
                while(id <= oMarkNum){
                    if(strcmp(oMark[id].value, "=") == 0){
                        id++;
                        mpType[mMarkNum] = id;
                        break;
                    }
                    id++;
                }
            }
        }
        uMark[i].pos = NewToOld[pos];
        uMark[i].depth = mMark[uMark[i].pos].depth;
        uMark[i].kind = tpNow;
        // uMark[i].deviation = mMark[uMark[i].pos].deviation;
        uMark[i].Belong = mMark[uMark[i].pos].Belong;

        if(pos != i && (tpNow == constType || tpNow == typeType)){
            // if(tpNow == typeType) printf("i = %d pos = %d\n", i, pos);
            int id = mpConst[uMark[i].pos]; //把当前的用第id个替换
            if(tpNow == typeType) id = mpType[uMark[i].pos];
            // printf("i = %d id = %d\n", i, id);
            Z_marktable &uM = uMark[i], oM = oMark[id];
            memcpy(uM.mark, oM.mark, sizeof(oM.mark));
            memcpy(uM.value, oM.value, sizeof(oM.value));
            uM.pos = 0;
            uM.kind = 0;
            uM.depth = 0;
            uM.kind = 0;
            // uM.deviation = 0;
        }

        // printf("%s %d\n", uMark[i].value, uMark[i].pos);
        // printf("%d\n", pos);
    }
    // int size[55000] = {0};
    // for(int i = 1; i <= mMarkNum; i++){
    //     if(mMark[i].kind >= 1 && mMark[i].kind <= 3){
    //         mMark[i].size = deviation[Belong[mMark[i].pos]];
    //     }
    //     printf("%s %d\n", mMark[i].value, mMark[i].size);
    // }

    // =======================求size====================
    for(int x = 1; x <= uMarkNum; x++){
        if(strcmp(uMark[x].value, "record") == 0){
            WorkSpace ws = workSpace[Belong[x]];
            int fr = ws.Begin, en = ws.End;
            int ii = 1;
            while(ii <= mMarkNum && mMark[ii].pos <= fr) ii++;
            int i = ii - 1;
            int j = i + 1;
            while(mMark[j].pos <= en && j <= mMarkNum) {
                if(mMark[j].kind != varType) {
                    j++;
                    continue;
                }
                if(mMark[i].depth  + 1 >= mMark[j].depth) {
                    // printf("%d %s %d\n", j, mMark[j].value, mMark[j].depth);
                    mMark[i].size++;
                    mMark[i].hav[mMark[i].size] = j;
                    mMark[j].belongPos = i;
                    mMark[j].deviation = mMark[i].size;
                }
                j++;
            }
        }
    }
    for(int i = 1; i <= mMarkNum; i++){
        if(mMark[i].kind == programType || mMark[i].kind == functionType || mMark[i].kind == procedureType){
            WorkSpace ws = workSpace[Belong[mMark[i].pos]];
            // printf("%d %d %d %s\n", i, ws.Begin, ws.End, mMark[i].value);
            // printf("i = %d %d\n", i, mMark[i].kind);
            if(mMark[i].kind == functionType || mMark[i].kind == procedureType){
                WorkSpace wsR1 = workSpace[ws.root];
                WorkSpace wsR2 = workSpace[wsR1.root];
                int idnow = wsR2.Begin + 1;
                mMark[i].belongPos = uMark[idnow].pos;
            }

            int fr = ws.Begin, en = ws.End;
            int j = i + 1;
            while(mMark[j].pos <= en && j <= mMarkNum) {
                if(mMark[j].kind >= 1 && mMark[j].kind <= 3) break;
                if(mMark[j].kind != varType) {
                    j++;
                    continue;
                }
                if(mMark[i].depth  + 1 >= mMark[j].depth) {
                    // printf("%d %s %d\n", j, mMark[j.value, mMark[j].depth);
                    mMark[i].size++;
                    mMark[i].hav[mMark[i].size] = j;
                    mMark[j].belongPos = i;
                    mMark[j].deviation = mMark[i].size;
                    mMark[j].depth = mMark[i].depth + 1;
                }
                j++;
            }
            // puts("");
        }
    }
    for(int i = 1; i <= uMarkNum; i++){
        uMark[i].size = mMark[uMark[i].pos].size;
        uMark[i].belongPos = mMark[uMark[i].pos].belongPos;
        uMark[i].deviation = mMark[uMark[i].pos].deviation;
        uMark[i].depth = mMark[uMark[i].pos].depth;
    }
    // for(int i = 1; i <= mMarkNum; i++){
    //     printf("%d %s\n", i, mMark[i].value);
    //     if(mMark[i].belongPos != 0){
    //         printf("%s\n", mMark[mMark[i].belongPos].value);
    //     }
    //     puts("\n");
    // }
    fprintf(mergeptr, "============mergedMarkTable================\n");
    for(int i = 1; i <= mMarkNum; i++){
        fprintf(mergeptr, "i:%d value:%s  pos:%d  depth = %d  size = %d dev = %d   bp = %d\n", i,mMark[i].value, mMark[i].pos, mMark[i].depth, mMark[i].size, mMark[i].deviation, mMark[i].belongPos);
    }
    fprintf(updateptr, "============updatedMarkTable===============\n");
    for(int i = 1; i <= uMarkNum; i++){
        fprintf(updateptr, "i:%d  %s  %s \n", i, uMark[i].mark, uMark[i].value);
    }
}

int main(){
    // ifstream fin("./MergedMarkTable.dat", ios::binary);
    // unsigned table_size;
    // fin.read((char*)&table_size, sizeof(unsigned));
    // fin.read((char*)&mMark, table_size);
    // for(int i = 1; i <= 10; i++) printf("%s %s\n", mMark[i].mark, mMark[i].value);
    Inititalize(); //初始化
    ReadMarkTable(); //从文件读入符号表
    CodeLayer(); //划分作用域
    ReplaceConstTypeAndMerge(); //替换const和type,id合并，只保留声明
    
    // ifstream fin("mmMark", ios::binary);
    // unsigned table_size;
    // fin.read((char*)&table_size, sizeof(unsigned));
    // fin.read((char*)&mmMark, table_size);
    // fin.close();

    // for(int i = 1; i <= mMarkNum; i++){
    //     if(mmMark[i].Belong != mMark[i].Belong) puts("Belong");
    //     else if(mmMark[i].Belong != mMark[i].Belong) puts("Belong");
    //     else if(mmMark[i].Belong != mMark[i].Belong) puts("Belong");
    //     else if(mmMark[i].Belong != mMark[i].Belong) puts("Belong");
    //     else if(mmMark[i].Belong != mMark[i].Belong) puts("Belong");
    //     else if(mmMark[i].Belong != mMark[i].Belong) puts("Belong");
    //     else
    // }


    //彦博请你生成词法分析后的程序
    for(int i = 1; i <= uMarkNum; i++) {
        uMark[i].mark;
        uMark[i].value;
        
        if(uMark[i].mark[0]=='r'||uMark[i].mark[0]=='a'||uMark[i].mark[0]=='m'||uMark[i].mark[0]=='l'||uMark[i].mark[0]=='n'||uMark[i].mark[0]=='i')
            fprintf(stdoutptr, "%s ", uMark[i].mark);// !!!!here
        else
        {
            fprintf(stdoutptr, "%s ", uMark[i].value);// !!!!here
            if(uMark[i].value[0]==';')
            {
                fprintf(stdoutptr, "\n");// !!!!here
            }
        }
            
    }
    

    mMark[0].Belong = mMarkNum;
    ofstream fout1(merge_dir, ios::binary);
    unsigned mMark_size = sizeof(mMark);
    fout1.write((char*)&mMark_size, sizeof(unsigned));
    fout1.write((char*)&mMark, sizeof(mMark));
    fout1.close();

    uMark[0].Belong = uMarkNum;
    ofstream fout2(update_dir, ios::binary);
    unsigned uMark_size = sizeof(uMark);
    fout2.write((char*)&uMark_size, sizeof(unsigned));
    fout2.write((char*)&uMark, sizeof(uMark));
    fout2.close();


    fclose(inptr);
    fclose(mergeptr);
    fclose(updateptr);
    fclose(stdoutptr);
    printf("生成符号表完成\n");
    return 0;
}
