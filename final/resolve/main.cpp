#include"def.h"
#include"loadInfo.h"
#include"resolve.h"

void init_info(){
    memset(mmMark, 0, sizeof(mmMark));
    memset(data_space, 0, sizeof(data_space));
}

int main() {
    load_info();
    do_resolve();
    return 0;
}
