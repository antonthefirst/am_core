#include "core/container.h"

#define POD(T)
#define STRUCT(ST) struct ST {
#define VAR(VT,VN,INIT,...) VT VN = INIT;
#define BUNCH(VT,VN,...) Bunch<VT> VN;
#define BIN(VT,VN,COUNT,...) Bin<VT,COUNT> VN;
#define END };