#include <cmath>
#include <string>
#include <vector>
#include "UTF8.h"
#include "utilitise.h"
#define nya std::



int roundCustom(float number) {
    if (number - round(number) < 0.9f)
        return floor(number);
    else
        return ceil(number);
}
nya string textFixedLength(const nya string& text, int length) {
    UTF8 utf8Text(text);
    if (utf8Text.getLength() >= length) {
        return utf8Text.substr(0, length - 1) + "…";
    }
    else {
        return utf8Text.getData() + nya string(length - utf8Text.getLength(), ' ');
    }
}

nya string toFixedNumber(int number, int n) {
    nya string numStr = nya to_string(number);
    int padding = n - static_cast<int>(numStr.length());
    if (padding > 0) {
        int leftPad = padding / 2;
        int rightPad = padding - leftPad;
        numStr = nya string(leftPad, ' ') + numStr + nya string(rightPad, ' ');
    }
    return numStr;
}
nya string calculeteCharProgressBar(int current, int max, int barLength, int curentHeight, bool horisontale) {
    UTF8 barChar(" ▁▂▃▄▅▆▇█"); // 0 → 8 niveaux
    UTF8 barCharHorisontale(" ▏▎▍▌▋▊▉█"); // 0 → 8 niveaux
    if (max <= 0 || barLength <= 0 || curentHeight < 0 || curentHeight > barLength)
        return " ";

    // ratio de remplissage total en "unités verticales"
    int totalUnits = barLength * 8; // 8 niveaux de barChar
    float sizePerUnit = (float)max / (float)totalUnits;

    int filledUnits = 0;
    if (current > 0)
        filledUnits = roundCustom(current / sizePerUnit);

    int segmentStart = (curentHeight - 1) * 8;
    int segmentEnd = curentHeight * 8;

    // Si rien n'est rempli ici → vide.
    if (filledUnits <= segmentStart)
        return " ";

    // Si segment rempli complètement → bloc plein
    if (filledUnits >= segmentEnd)
        return "█";

    // Sinon : rempli partiellement → choisir le bon niveau dans barChar
    int level = filledUnits - segmentStart; // 1 → 7
    if (level < 0) level = 0;
    if (level > 7) level = 7;

    // barChar[0] est " " donc on commence à 1 pour ▁
    if (horisontale)
        return barCharHorisontale.getCharAt(level + 1);
    else
        return barChar.getCharAt(level + 1);
}

nya string lightCalculateProgressBar(int current, int max) {
    //bar of 9 characters
    UTF8 bar(" ▁▂▃▄▅▆▇█");
    if (max <= 0 || current <= 0)
        return " ";
    int box = bar.getLength() - 1;
    float sizePerUnit = max / box;
    int filledUnits = roundCustom(current / sizePerUnit);
    if (filledUnits <= 0)
        return " ";
    if (filledUnits >= box)
        return bar.getCharAt(box);
    return bar.getCharAt(filledUnits);
}

