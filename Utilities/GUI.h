//
// Created by DeJorgiK on 20/09/2024.
//

#ifndef PAG_1_GUI_H
#define PAG_1_GUI_H

namespace PAG {

    class GUI {
    private:
        static GUI* gui_instance;
        GUI();
    public:
        virtual ~GUI();
        static GUI& getInstance();
        void guiInit();
        void shutDown();
        void newFrame();
        void render();

        void drawMessage(float posX,float posY,float fontScale,const char *title,const char *text);
        void drawColorWheel(float posX,float posY,float fontScale,float *col,const char *title,const char *text);
    };

} // PAG

#endif //PAG_1_GUI_H
