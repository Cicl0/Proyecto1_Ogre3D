#include "IG2Project.h"

int main(int argc, char* argv[]){
    
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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
        // _NORMAL_BLOCK corresponde a las reservas hechas por tu aplicación (new / malloc)
        size_t numeroDeLeaks = diferencia.lCounts[_NORMAL_BLOCK];
        size_t bytesPerdidos = diferencia.lSizes[_NORMAL_BLOCK];

        std::cout << "Memory leaks detectados: " << numeroDeLeaks << "--------------------------------------------------------------\n";
        std::cout << "Total de bytes sin liberar: " << bytesPerdidos << " bytes----------------------------------------------------\n";

        // Opcional: imprime en la ventana Output de VS solo los leaks ocurridos entre ambos puntos
        _CrtMemDumpStatistics(&diferencia);
    }
    else {
        std::cout << "0 memory leaks.\n";
    }

    return 0;
}