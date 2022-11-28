// Lab 12: Part Two - Implement checkpoints, command history, undo and redo commands using Command and Memento design patterns
// Isaac Fowler
//
// history.cpp

#include <iostream>
#include <vector>
#include <string>
#include <stack>

using std::vector; using std::stack; using std::string;
using std::cout; using std::cin; using std::endl;

// receiver
class Document {
public:

   Document(const vector <string> & lines={}): lines_(lines){}

   // actions
   void insert(int line, const string &str) {
      const int index = line-1;
      if (index <= lines_.size())
	 lines_.insert(lines_.begin() + index, str); 
      else
	 cout << "line out of range" << endl; 
   }

   string remove(int line) {
      const int index = line-1;
      string deletedLine="";
      if(index < lines_.size()){
	 deletedLine = lines_[index];
	 lines_.erase(lines_.begin() + index); 
      }else
	 cout << "line out of range" << endl; 
      return deletedLine;
   } 

   void show() {
      for(int i = 0; i < lines_.size(); ++i)
	 cout << i + 1 << ". " << lines_[i] << endl; 
   }

private:
   vector<string> lines_; 
}; 

// abstract command
class Command{
public:
   Command(Document *doc) : doc_(doc){}
   virtual void execute() = 0; 
   virtual void unexecute() = 0;
   virtual void print() const = 0; 
   virtual ~Command(){}
protected:
   Document *doc_; 
}; 

// two concrete commands
class InsertCommand : public Command {
public:
   InsertCommand(Document *doc, int line, const string &str): 
      Command(doc), line_(line),  str_(str) {}
   void execute() override { doc_->insert(line_, str_);}
   void unexecute() override {doc_->remove(line_);}
   void print() const override { cout << "Insert \"" << str_ << "\" at line " << line_; }
private:
   int line_; 
   string str_; 
}; 


class EraseCommand : public Command {
public:
   EraseCommand(Document *doc, int line): 
      Command(doc), line_(line), str_("") {}
   void execute() override {str_ = doc_->remove(line_);}
   void unexecute() override {doc_->insert(line_, str_);}
   void print() const override { cout << "Erase line " << line_; }
private:
   int line_;
   string str_;
};

// client
class DocumentWithHistory{
public:
   DocumentWithHistory(const vector<string> & text={}) : doc_(text){}

   void insert(int line, string str) {
      Command *com = new InsertCommand(&doc_, line, str); 
      com->execute(); 
      doneCommands_.push(com); 
   }

   void erase(int line){
      Command *com = new EraseCommand(&doc_, line);
      com->execute();
      doneCommands_.push(com);
   }

   void undo() {
      if(doneCommands_.size() != 0) {
	 Command *com = doneCommands_.top(); 
	 doneCommands_.pop(); 
	 com->unexecute();
	 //delete com; // don't forget to delete command
      }else
	 cout << "no commands to undo" << endl; 
   }

   void purge() {
	   while(!doneCommands_.empty())
		   doneCommands_.pop();
   }

   	void show() {doc_.show();}
	class Memento *createMemento() const;  
   	void rollBack(class Memento *);

	void printHistory() {
		stack<Command*> history = doneCommands_;
		stack<Command*> reverse;

		//cant traverse a stack so have to pop every element
		while (!history.empty()) {
			reverse.push(history.top());
			history.pop();
		}
		int i = 1;
		while (!reverse.empty()) {
			Command *cmd = reverse.top();
			cout << i << ": ";
			cmd->print();
			cout << endl;
			reverse.pop();
			++i;
		}
	}

	void redo(int line) {
		int i = 1;
		stack<Command*> history = doneCommands_;
		stack<Command*> reverse;
		while(!history.empty()) {
			reverse.push(history.top());
			history.pop();
		}
		if (line <= reverse.size()) {
			while (i < line) {
				reverse.pop();
				++i;
			}
			Command *cmd = reverse.top();
			cmd->execute();
			doneCommands_.push(cmd);
		} else {
			cout << "Line out of bounds." << endl;
		}
	}

private:
   Document doc_;
   std::stack<Command*> doneCommands_; 
};

class Memento {
public:
   Memento(const DocumentWithHistory &doc): doc_(doc) {}
   const DocumentWithHistory &getState() const {return doc_;}  
private:
   const DocumentWithHistory doc_;
};
 
Memento *DocumentWithHistory::createMemento() const {
   return new Memento(*this);
}

void DocumentWithHistory::rollBack(Memento *mem) {
   *this = mem->getState();
}

int main() {
   DocumentWithHistory his({
	 "Lorem Ipsum is simply dummy text of the printing and typesetting",
	 "industry. Lorem Ipsum has been the industry's standard dummy text",
	 "ever since the 1500s, when an unknown printer took a galley of",
	 "type and scrambled it to make a type specimen book. It has",
	 "survived five centuries."}); 
	Memento *checkpoint = nullptr;
   char option;
   do{
      his.show(); 
      cout << endl;

      cout << "Enter option (i)nsert line, (e)rase line, (u)ndo last command, (c)heckpoint, roll(b)ack, (h)istory, (r)edo command: ";
      cin >> option; 

      int line; string str; 
      switch(option) {
      case 'i':
	 cout << "line number to insert: "; 
	 cin >> line; 
	 cout << "line to insert: "; 
	 cin.get(); // removes end-of-line character so getline() works correctly
	 getline(cin, str); 
	 his.insert(line, str); 
	 break;
 
      case 'e':
	 cout << "line number to remove: ";
	 cin >> line;
	 his.erase(line);
	 break;

      case 'u':
	 his.undo(); 
	 break; 

      case 'c':
	 his.purge();
	 checkpoint = his.createMemento();
	 break;

      case 'b':
	 if (checkpoint != nullptr) {
	 	his.rollBack(checkpoint);
	 	delete checkpoint;
	 } else
		 cout << "Error: No checkpoint created to rollback to!" << endl;
	 break;

      case 'h':
	 cout << "----------[ History ]----------" << endl;
	 his.printHistory();
	 cout << "-------------------------------" << endl;
	 break;

      case 'r':
	 his.printHistory();
	 cout << "Enter the command number you wish to redo: ";
	 cin >> line;
	 his.redo(line);
	 break;
      }

   }while(option == 'i' || option == 'e' || option == 'u' || option == 'c' || option == 'b' || option == 'h' || option == 'r');
}
