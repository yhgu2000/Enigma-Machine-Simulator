#include "palettes.h"

QPalette g_badPalette(QColor(170,0,0));

QPalette g_goodPalette(QColor(0,170,0));

QPalette g_warningPalette(QColor(255,255,0));

static bool initPalettes();
static volatile bool f = initPalettes();
static bool initPalettes()
{
    g_badPalette=QColor(170,0,0);
    g_badPalette.setColor(QPalette::WindowText,Qt::white);
    return f;
}
