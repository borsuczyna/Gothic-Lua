namespace GOTHIC_ENGINE {
    class zColor {
    public:
        static int ToColor(int r, int g, int b, int a) {
            return (r << 24) | (g << 16) | (b << 8) | a;
        }

        static void FromColor(int hexValue, int& r, int& g, int& b, int& a) {
            r = (hexValue >> 24) & 0xFF;
            g = (hexValue >> 16) & 0xFF;
            b = (hexValue >> 8) & 0xFF;
            a = hexValue & 0xFF;
        }
    };
}