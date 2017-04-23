// A multiresolution triangular grid.
// Elements are indexed using a granularity number and two coordinates.
// The grid is internally subdivided as needed to complete the operation.
// After an operation, neighboring elements containing all the same value will
// be merged (normalized) to minimize storage needs.

// A collection of triangles
template <class T> class TriMesh
{
  uint size
  private:
    T[size] data;
  public:
    T& operator[](size_t i)
    {
        return data[i];
    }
    const T& operator[](size_t i) const
    {
        return this->data[i];
    }
    void insert(size_t i, const T& new_data) {
        data[i] = new_data;
    }
};

class Hexagon
{
    const hexsides=6;
  private:
    Triangle[hexsides] hexdata;
    uint hexorientation;
  public:
    Hexagon(Triangle basetriangle, uint orientation){
        hexorientation=orientation;
        hexdata[0]=basetriangle;
        //Go clockwise to fill hexagon from origin hex
        firststep=(orientation+hexsides/Triangle.sides)%hexsides;
        for(int i=0; i<5; i++){
            hexdata[i+1]=hexdata[i].adjacent(firststep+i);
        }
    }
    Triangle operator[](size_t i)
    {
        return hexdata[(i-hexorientation)%hexsides];
    }
    const sides{
        return hexsides;
    }
}

class Parallelogram
{
}

   class Line

   //Should I derive Triangle from TriGrid or something?
   class Triangle{
     public:
       const sides=3;
       Triangle adjacent(uint i){
           switch(i%6){
             case 0:
               //Return triangle to lower-left
               break;
             case 1:
               //Return triangle to upper-left
               break;
             case 2:
               //Return triangle above
               break;
             case 3:
               //Return triangle to upper-right
               break;
             case 4:
               //Return triangle to lower-right
               break;
             case 5:
               //Return triangle below
               break;
           }
