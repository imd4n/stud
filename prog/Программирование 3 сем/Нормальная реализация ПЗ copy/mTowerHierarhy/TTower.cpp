void Point::drawObj(u_char r, u_char g, u_char b) {
// Проходим по каждому байту
    u_char * ptr = bitMap;
    int thisBit = 0;
    for (int i = 0; i < mapN*mapM/8; ++i) {
        // Разыменовываем указатель, чтобы получить значение байта
        unsigned char current_byte = *ptr; 
        for (int j = 7; j >= 0; --j) {
            thisBit++;
            if(((current_byte >> j) & 1)) {// Выводим биты
                int thisN = thisBit/mapN;
                int thisM = thisBit%mapM;
                
                SetPixel(hdc, x+thisM, y+thisN, RGB(r, g, b));
            }
        }
        
        // Переходим к следующему байту
        ptr++; 
    }
}