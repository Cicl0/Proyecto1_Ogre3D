#include "IG2Project.h"

int main(int argc, char* argv[]){
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(892486);

    _CrtMemState estadoInicial, estadoFinal, diferencia;
    _CrtMemCheckpoint(&estadoInicial);

    {
        IG2Project app;
        try {
            app.initApp();
            app.getRoot()->startRendering();
        }
        catch (Ogre::Exception& e) {
            Ogre::LogManager::getSingleton().logMessage("An exception has occured: " + e.getFullDescription() + "\n");
        }
        app.closeApp();
    }

    _CrtMemCheckpoint(&estadoFinal);

    if (_CrtMemDifference(&diferencia, &estadoInicial, &estadoFinal)) {
        std::cout << "Memory leaks detectados: " << diferencia.lCounts[_NORMAL_BLOCK] << "\n";
        std::cout << "Bytes sin liberar: " << diferencia.lSizes[_NORMAL_BLOCK] << "\n";

        _CrtMemDumpStatistics(&diferencia);

        // Vuelca CADA bloque vivo desde estadoInicial, con su número {N}
        //_CrtMemDumpAllObjectsSince(&estadoInicial);
    }

    return 0;
}