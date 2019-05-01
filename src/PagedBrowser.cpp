#include "PagedBrowser.h"
#include <math.h>

template <class T>
PagedBrowser<T>::PagedBrowser(float w, float h, float elW, float elH, std::vector<std::unique_ptr<T>> els)
: width(w), height(h), elementW(elW), elementH(elH), elements(els) {
    elementsPerLine = floor(width / elementW);
    elementsPerCol = floor(height / elementH);
    numPages = (int)elements.size() / (elementsPerLine * elementsPerCol);
    
    
}


