
class distanceMap
{
 public:
  
  distanceMap():width_(0),height_(0),map_(0){}

    double** generate(int x, int y,double resolution,int width, int height);

  double** map();

  int width();

  int height();

  ~distanceMap();

 private:

  int width_;

  int height_;

  double** map_;
};
