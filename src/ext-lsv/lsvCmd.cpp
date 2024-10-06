#include "base/abc/abc.h"
#include "base/main/main.h"
#include "base/main/mainInt.h"


static int Lsv_CommandPrintNodes(Abc_Frame_t* pAbc, int argc, char** argv);
static int Lsv_CommandPrintCut(Abc_Frame_t* pAbc, int argc, char** argv);

void init(Abc_Frame_t* pAbc) {
  Cmd_CommandAdd(pAbc, "LSV", "lsv_print_nodes", Lsv_CommandPrintNodes, 0);
  Cmd_CommandAdd(pAbc, "LSV", "lsv_printcut", Lsv_CommandPrintCut, 0);
}

void destroy(Abc_Frame_t* pAbc) {}

Abc_FrameInitializer_t frame_initializer = {init, destroy};

struct PackageRegistrationManager {
  PackageRegistrationManager() { Abc_FrameAddInitializer(&frame_initializer); }
} lsvPackageRegistrationManager;

void Lsv_NtkPrintNodes(Abc_Ntk_t* pNtk) {
  Abc_Obj_t* pObj;
  int i;
  Abc_NtkForEachNode(pNtk, pObj, i) {
    printf("Object Id = %d, name = %s\n", Abc_ObjId(pObj), Abc_ObjName(pObj));
    Abc_Obj_t* pFanin;
    int j;
    Abc_ObjForEachFanin(pObj, pFanin, j) {
      printf("  Fanin-%d: Id = %d, name = %s\n", j, Abc_ObjId(pFanin),
             Abc_ObjName(pFanin));
    }
    if (Abc_NtkHasSop(pNtk)) {
      printf("The SOP of this node:\n%s", (char*)pObj->pData);
    }
  }
}

int Lsv_CommandPrintNodes(Abc_Frame_t* pAbc, int argc, char** argv) {
  Abc_Ntk_t* pNtk = Abc_FrameReadNtk(pAbc);
  int c;
  Extra_UtilGetoptReset();
  while ((c = Extra_UtilGetopt(argc, argv, "h")) != EOF) {
    switch (c) {
      case 'h':
        goto usage;
      default:
        goto usage;
    }
  }
  if (!pNtk) {
    Abc_Print(-1, "Empty network.\n");
    return 1;
  }
  Lsv_NtkPrintNodes(pNtk);
  return 0;

usage:
  Abc_Print(-2, "usage: lsv_print_nodes [-h]\n");
  Abc_Print(-2, "\t        prints the nodes in the network\n");
  Abc_Print(-2, "\t-h    : print the command usage\n");
  return 1;
}





void Lsv_EnumerateCuts(Abc_Obj_t * pObj,Vec_Ptr_t * vCuts, Abc_Obj_t * pObjo,int k) {

    

    Vec_PtrPush(vCuts,pObj);
                      
    if(Vec_PtrSize(vCuts)>k) {
        return;
    } 
    Abc_Print(1, "%d:", Abc_ObjId(pObjo));
    Abc_Obj_t * pLeaf;
    int l;
    Vec_PtrForEachEntry(Abc_Obj_t *, vCuts, pLeaf, l) {
      Abc_Print(1, " %d", Abc_ObjId(pLeaf));
    }
    Abc_Print(1, "\n");
    

    if (Abc_ObjIsCi(pObj)){
        return;
    }
    Vec_PtrPop(vCuts);
    Vec_PtrPush(vCuts, Abc_ObjFanin1(pObj));
    Lsv_EnumerateCuts(Abc_ObjFanin0(pObj), vCuts,pObjo, k); 
    Vec_PtrPop(vCuts);
    Vec_PtrPush(vCuts, Abc_ObjFanin0(pObj));
    Lsv_EnumerateCuts(Abc_ObjFanin1(pObj), vCuts,pObjo, k);
    

    
    return;

}

int Lsv_CommandPrintCut(Abc_Frame_t * pAbc, int argc, char ** argv) {

    int k = atoi(argv[1]); 
    Abc_Ntk_t * pNtk = Abc_FrameReadNtk(pAbc);
    Abc_Obj_t * pObj;
    int i;
    Abc_NtkForEachNode(pNtk, pObj, i) {
        Vec_Ptr_t * vCuts = Vec_PtrAlloc(6);  // dynamic array
        Lsv_EnumerateCuts(pObj, vCuts,pObj, k);

        
    }
    return 0;
}

