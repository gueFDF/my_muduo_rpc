#include "mprpcappclication.h"

void MprpcApplication::Init(int argc, char **argv)
{

}
MprpcApplication &MprpcApplication::GetInitstance()
{
    static MprpcApplication app;
    return app;
}
