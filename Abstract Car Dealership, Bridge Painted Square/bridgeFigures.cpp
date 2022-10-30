//Lab 9: Part 2 - Bridge Design Pattern. Bridged over Figure --> Fill
//
//Isaac Fowler
//bridgeFigures.cpp

#include <iostream>

using std::cout; using std::endl; using std::cin;

// abstract body
class Fill{
public:
   //Fill(char fillChar):fillChar_(fillChar){}
   Fill(char borderChar, char internalChar): borderFillChar_(borderChar), internalFillChar_(internalChar) {}
   virtual char getBorder()=0;
   virtual char getInternal()=0;
   virtual ~Fill() {}
protected:
   char borderFillChar_;
   char internalFillChar_;
};

// concrete body
class Hollow: public Fill{
public:
   Hollow(char fillChar):Fill(fillChar, ' '){}
   char getBorder() override {return borderFillChar_;}
   char getInternal() override {return internalFillChar_;}
   ~Hollow() override {}
};


// another concrete body
class Filled: public Fill {
public:
   Filled(char fillChar):Fill(fillChar, fillChar){}
   char getBorder() override {return borderFillChar_;}
   char getInternal() override {return internalFillChar_;}
   ~Filled() override {}
};

// class I added - Uses Fill constructor with separate characters for border and internal filling
class FullyFilled: public Fill {
public:
	FullyFilled(char borderChar, char internalChar):Fill(borderChar, internalChar){}
	char getBorder() override {return borderFillChar_;}
	char getInternal() override {return internalFillChar_;}
	~FullyFilled() override {}
};

// class I added - Uses Fill constructor with separate characters for border and internal filling
// Also getBorder() and getInternal() will return randomized characters.
class RandomFilled: public Fill {
public:
	RandomFilled(char borderChar, char internalChar): Fill(borderChar, internalChar) {}
	char getBorder() override {return ((rand()%2 == 0) ? borderFillChar_ : internalFillChar_);}
	char getInternal() override {return ((rand()%2 == 0) ? borderFillChar_ : internalFillChar_);}
	~RandomFilled() override {}
};

// abstract handle
class Figure {
public:
   Figure(int size, Fill* fill): size_(size), fill_(fill){}
   virtual void draw() =0;
   virtual ~Figure(){}
protected:
   int size_;
   Fill *fill_;
};

// concrete handle
class Square: public Figure{
public:
   Square(int size, Fill* fill): Figure(size, fill){}
   void draw() override;

};

void Square::draw(){
   for(int i=0; i < size_; ++i){
      for(int j=0; j < size_; ++j)
	 if(i==0 || j==0 || i==size_-1 || j==size_-1 )
	    cout << fill_ -> getBorder();
	 else
	    cout << fill_ -> getInternal();
      cout << endl;
   }
}

int main(){

   	Fill* hollowPaintY = new Hollow('Y');
   	Fill* filledPaintStar = new Filled('*');
   	Fill* fullyFilledPaint = new FullyFilled('X', '.');
   	Fill* randomFilledPaint = new RandomFilled('-', 'M');
   
   	Figure *smallBox = new Square(6, filledPaintStar);
  	Figure *bigBox = new Square(10, hollowPaintY);
   	Figure *ffBox = new Square(8, fullyFilledPaint);
	Figure *rfBox = new Square(9, randomFilledPaint);
	
   	cout << endl << "Hollow:" << endl << endl;
   	smallBox->draw();
   	cout << endl << "Filled:" << endl << endl;
	bigBox -> draw();
   	cout << endl << "FullyFilled:" << endl << endl;
   	ffBox -> draw();
   	cout << endl << "RandomFilled:" << endl << endl;
   	rfBox -> draw();  
}
