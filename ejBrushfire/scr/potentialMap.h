#include <vector>

class distanceMap;
class brushfireMap;

using namespace std;

class pos
{
 public:
  pos(int a, int b):y(a),x(b){}
  int x;
  int y;
};

class potentialMap
{
 public:
  
  potentialMap():width_(0),height_(0),map_(0),pathLen_(0){}

  double** generate(distanceMap& dm, brushfireMap& bm);

  vector<pos*> findPath(int i, int j);

  double** map();

  int width();

  int height();
  
  vector<pos*> path();

  int pathLen();

  ~potentialMap();

 private:

  int width_;

  int height_;

  double** map_;

  int pathLen_;

  vector<pos*> path_;
};
