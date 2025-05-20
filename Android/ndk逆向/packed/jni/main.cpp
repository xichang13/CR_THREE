#include <stdio.h>
#include <dlfcn.h>

int main(){
    void* handle = dlopen("/data/local/tmp/libalgorith_packed4.so", RTLD_NOW);
    printf("handle %p %s\n", handle, dlerror());
    dlclose(handle);
    return 0;
}