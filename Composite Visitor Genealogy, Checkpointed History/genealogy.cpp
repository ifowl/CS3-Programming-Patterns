// Lab 12: Part One - Traverse and compare Genealogy Tree using Visitor and Composite Design Patterns
// Isaac Fowler
//
// genealogy.cpp

#include <iostream>
#include <string>
#include <vector>

using std::cout; using std::cin; using std::endl;
using std::string; 
using std::vector;
using std::getline;

class Person{ // component
public:
   Person(string firstName, Person *spouse, Person *father, Person *mother):
      firstName_(firstName), spouse_(spouse), father_(father), mother_(mother){}
   const string & getFirstName(){return firstName_;}
   virtual const string getFullName() =0;
   Person *getSpouse(){return spouse_;}
   void setSpouse(Person *spouse){spouse_=spouse;}
   Person *getFather(){return father_;}

   virtual void accept(class PersonVisitor *)=0;  
   virtual ~Person(){}
private:
   const string firstName_;
   Person *spouse_;
   Person *father_;
   Person *mother_;
};


// leaf
// man has a last name 
class Man: public Person{
public:
   Man(string lastName, string firstName, Person *spouse, 
       Person *father, Person *mother): 
      lastName_(lastName), 
      Person(firstName, spouse, father, mother){}
   const string & getLastName(){return lastName_;}
   const string getFullName() override { return getFirstName() + " " + lastName_; }
   void accept(class PersonVisitor *visitor) override;
   void setVisited() { visited_ = true; }
   bool getVisited() { return visited_; }
private:
   const string lastName_;
   bool visited_ = false;
};

// composite
// woman has a list of children
class Woman: public Person{
public: 
   Woman(vector<Person *> children, 
       string firstName, 
       Person *spouse, 
       Person *father, Person *mother): 
       children_(children),
       Person(firstName, spouse, father, mother){}
   const vector<Person *> & getChildren() {return children_;}
   void setChildren(const vector<Person *> &children){ children_ = children;}
   void accept(class PersonVisitor *visitor) override;
   const string getFullName() override {
	   if (getSpouse() != nullptr )
		   return (getFirstName() + " " + static_cast<Man*>(getSpouse())->getLastName());
	   else if (getFather() != nullptr)
		   return (getFirstName() + " " + static_cast<Man*>(getFather())->getLastName());
	   else
		   return (getFirstName() + "Doe");
   }
   void setVisited() { visited_ = true; }
   bool getVisited() { return visited_; }
private:
   vector<Person *> children_;
   bool visited_ = false;
}; 

// abstract visitor
class PersonVisitor{
public:
   virtual void visit(Man*)=0;
   virtual void visit(Woman*)=0;
   virtual ~PersonVisitor(){}
};

// composite methods that define traversal
void Man::accept(PersonVisitor *visitor) {
    visitor->visit(this);
    if (getSpouse() != nullptr && !static_cast<Woman*>(getSpouse())->getVisited())
    	getSpouse()->accept(visitor);
}

void Woman::accept(PersonVisitor *visitor){ 
   // children traversal through mother only
   // father's children are not traversed so as not 
   // to traverse them twice: for mother and father

   visitor->visit(this);

    if (getSpouse() != nullptr && !static_cast<Man*>(getSpouse())->getVisited())
    	getSpouse()->accept(visitor);
   
   // traversing descendants
   for(auto child : children_) 
      child->accept(visitor);   
}

// concrete visitors
// the last name for a man is stored in object
// the last name a woman is determined by her 
// spouse if she is married
// or by her father if she is not
class NamePrinter: public PersonVisitor{
public:
   void visit(Man *m) override {
      cout << m->getFirstName() << " " << m->getLastName() << endl;
      m->setVisited();
   }
   void visit(Woman *w) override {
      cout << w->getFirstName() << " ";
      if (w->getSpouse() != nullptr)
	 cout << static_cast<Man *>(w->getSpouse())->getLastName();
      else if (w->getFather() != nullptr)
	 cout << static_cast<Man *> (w->getFather())->getLastName();
      else
	 cout << "Doe";
      w->setVisited();
      cout << endl;
   }
};


class ChildrenPrinter: public PersonVisitor{
public:
   void visit(Man *m) override {
      cout << m->getFirstName() << ": ";
      Woman *spouse = static_cast<Woman *>(m->getSpouse());
      if(spouse != nullptr)
	 printNames(spouse->getChildren());
      cout << endl;
   }
   void visit(Woman *w) override {
      cout << w->getFirstName() << ": ";
      printNames(w->getChildren());
      cout << endl;
   }
private:
   void printNames(const vector<Person *> &children){
      for(const auto c: children)
	 cout << c->getFirstName() << ", ";
   }
};

class CloseRelatives: public PersonVisitor{
public:
	CloseRelatives(string first, string second): personOne_(first), personTwo_(second) {}
	void printRelated() {
		if (relatives) cout << personOne_ << " and " << personTwo_ << " are close relatives." << endl;
		else cout << personOne_ << " and " << personTwo_ << " are just friends." << endl;
	}
   	void visit(Man *m) override {
		if (m->getFullName() == personOne_) { //if current node is first person
			if (m->getFullName() == personTwo_) { //Self-check
				relatives = true;
			} else if (m->getFather() != nullptr) {
				auto dad = m->getFather();
			       	if (dad->getFullName() == personTwo_) { //Father
					relatives = true;
				} else if (dad->getSpouse()->getFullName() == personTwo_) { //Mother
					relatives = true;
				} else if (dad->getFather() != nullptr) {
					for (auto uncleAunt : static_cast<Woman*>(dad->getFather()->getSpouse())->getChildren()) { //Paternal Aunts/Uncles
						if (uncleAunt->getFullName() == personTwo_)
							relatives = true;
					}
				} else if (dad->getSpouse()->getFather() != nullptr) {
					for (auto uncleAunt : static_cast<Woman*>(dad->getSpouse()->getFather()->getSpouse())->getChildren()) { //Maternal Aunts/Uncles
						if (uncleAunt->getFullName() == personTwo_)
							relatives = true;
					}
				} else {
					for (auto sibling : static_cast<Woman*>(dad->getSpouse())->getChildren()) { //Siblings
						if (sibling->getFullName() == personTwo_)
							relatives = true;
					}
				}
			} else if (m->getSpouse() != nullptr) {
				for (auto child : static_cast<Woman*>(m->getSpouse())->getChildren()) { //Children
					if (child->getFullName() == personTwo_)
						relatives = true;
				}
			}
		}
		m->setVisited();
   	}
   	void visit(Woman *w) override {
		if (w->getFullName() == personOne_) {
			if (w->getFullName() == personTwo_) { //self-check
				relatives = true;
			} else if (w->getFather() != nullptr) {
				auto dad = w->getFather();
				if (dad->getFullName() == personTwo_) { //Father
					relatives = true;
				} else if (dad->getSpouse()->getFullName() == personTwo_) { //Mother
					relatives = true;
				} else if (dad->getFather() != nullptr) {
					for (auto uncleAunt : static_cast<Woman*>(dad->getFather()->getSpouse())->getChildren()) { //Paternal Aunts/Uncles
						if (uncleAunt->getFullName() == personTwo_)
							relatives = true;
					}
				} else if (dad->getSpouse()->getFather() != nullptr) {
					for (auto uncleAunt : static_cast<Woman*>(dad->getSpouse()->getFather()->getSpouse())->getChildren()) { //Maternal Aunts/Uncles
						if (uncleAunt->getFullName() == personTwo_)
							relatives = true;
					}
				} else {
					for (auto sibling : static_cast<Woman*>(dad->getSpouse())->getChildren()) { //Siblings
						if (sibling->getFullName() == personTwo_)
							relatives = true;
					}
				}
			} else if (w->getSpouse() != nullptr) {
				for (auto child : w->getChildren()) { //Children
					if (child->getFullName() == personTwo_)
						relatives = true;
				}
			}
		}
		w->setVisited();
   	}
private:
	bool relatives = false;
   	string personOne_;
	string personTwo_;
};

// demonstrating the operation
int main(){

   // setting up the genealogical tree      
   // the tree is as follows
   //    
   //
   //       James Smith  <--spouse-->   Mary 
   //	                                  |
   //	                                 children -------------------------
   //	                                  |              |                |
   //	                                  |              |                |
   //	   William Johnson <--spouse-> Patricia      Robert Smith       Linda
   //	                                  |
   //	                                 children------------
   //	                                  |                 |
   //                                     |                 |
   //	   Jennifer  <--spouse-->  Michael Johnson      Barbara
   //	       |
   //	     children
   //	       |
   //          |
   //	     Susan


   // first generation
   Man *js = new Man("Smith", "James", nullptr, nullptr, nullptr);
   Woman *ms = new Woman({}, "Mary", nullptr, nullptr, nullptr);
   ms->setSpouse(js); js->setSpouse(ms);

   // second generation
   Woman *ps = new Woman({}, "Patricia",  nullptr, js, ms);
   Man *wj = new Man("Johnson", "William", nullptr, nullptr, nullptr);
   ps->setSpouse(wj); wj->setSpouse(ps);

   vector<Person *> marysKids  = {ps,
			          new Man("Smith", "Robert", nullptr, js, ms),
			          new Woman({}, "Linda", nullptr, js, ms)};
   ms->setChildren(marysKids);

   // third generation
   Man *mj = new Man("Johnson", "Michael", nullptr, wj, ps);
   vector<Person *> patsKids   = {mj, new Woman({}, "Barbara", nullptr, wj, ps)}; 
   ps->setChildren(patsKids);

   Woman *jj = new Woman({}, "Jennifer", nullptr, nullptr, nullptr);
   vector<Person *> jensKids = {new Woman({}, "Susan", nullptr, mj ,jj)};

   jj->setSpouse(mj); mj->setSpouse(jj);
   jj->setChildren(jensKids);
   

   // defining two visitors
   ChildrenPrinter *cp = new ChildrenPrinter;
   NamePrinter *np = new NamePrinter;

   // executing the traversal with the composite
   // and the specific visitor

   /*
   cout << "\nNAME LIST\n";
   ms->accept(np);
   cout << endl << endl;
   
   
   cout << "CHILDREN LIST\n";
   ms->accept(cp);

   
   cout << "\nJAMES' CHILDREN\n";
   js->accept(cp);

   */
   	string first; string second;
   	cout << "Enter first candidate: ";
	getline(cin, first);
	cout << "Enter second candidate: ";
	getline(cin, second);
	cout << endl;
  
	CloseRelatives *cr = new CloseRelatives(first, second);
	ms->accept(cr);
	cr->printRelated();
  
}
