// Lab 10: Part 1 - class adapter pattern
// Isaac Fowler
// 
// ClassAdapter.cpp

#include <iostream>

using std::cout; using std::endl; using std::cin;

// base interface
class Figure {
public:
   virtual void draw()=0;
   virtual int size()=0;
   virtual void resize(int)=0;
   virtual ~Figure(){}
};

// adaptee/implementer
class LegacyRectangle{
public:
   LegacyRectangle(int topLeftX, 
		   int topLeftY, 
		   int bottomRightX, 
		   int bottomRightY): 
      topLeftX_(topLeftX), 
      topLeftY_(topLeftY), 
      bottomRightX_(bottomRightX), 
      bottomRightY_(bottomRightY){}

   void oldDraw(){
      for(int i=0; i < bottomRightY_; ++i){
	 for(int j=0; j < bottomRightX_; ++j)
	    if(i >= topLeftY_  && j >= topLeftX_ )
	       cout << '*';
	    else
	       cout << ' ';
	 cout << endl;
      }
   }

   void move(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY) {
	topLeftX_= topLeftX; topLeftY_ = topLeftY; bottomRightX_ = bottomRightX; bottomRightY_ = bottomRightY;
   }
   int getTopLeftX() { return topLeftX_; }
   int getTopLeftY() { return topLeftY_; }
   int getBottomRightX() { return bottomRightX_; }
   int getBottomRightY() { return bottomRightY_; }
// defining top/left and bottom/right coordinates 
private: 
   int topLeftX_;
   int topLeftY_;
   int bottomRightX_;
   int bottomRightY_;
};

// adapter uses multiple inheritance to inherit
// interface and implementation
class SquareAdapter: public Figure, 
		     private LegacyRectangle {
public:
   SquareAdapter(int size): 
      LegacyRectangle(0,0,size,size){};
   void draw() override {
      oldDraw();
   }
   int size() override {
	   return getBottomRightX()-getTopLeftX();
   }
   void resize(int newSize) override {
	   int topLeft = getTopLeftX();
	   move(topLeft, topLeft, newSize, newSize);
   }

};


int main(){
	cout << "Enter a size for the square: ";
	int size;
	cin >> size;
   	Figure *square = new SquareAdapter(size);
   	square->draw();
	cout << "Enter a new size: ";
	int newsize;
	cin >> newsize;
   	square->resize(newsize);
   	square->draw();
}
