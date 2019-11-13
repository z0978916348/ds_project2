#include <iostream>
#include <utility> // for pair<T,T>
#include <fstream>
#include "queue.h"
#include "stack.h"
int count = 0;
using namespace std;
ifstream inFile("floor.data", ios::in);
ofstream outFile("final.path", ios::out);
struct floor{
    int f_row;
    int f_col;
    int f_weight;
    floor *prev;   // link path 
    floor(): f_row(0), f_col(0), f_weight(0), prev(nullptr){}
    floor(int r, int c, int w): f_row(r), f_col(c), f_weight(w), prev(nullptr){}
};

class AnsNode{
    friend class Ans;
private:
    int row;
    int col;
    AnsNode* next;
public:
    AnsNode():row(0),col(0), next(nullptr){};
    AnsNode(int r, int c,AnsNode* next):row(r),col(c), next(next){};
};

class Ans{
private:
    AnsNode* head = nullptr;
    AnsNode* tail = nullptr;
public:
    Ans(): head(nullptr), tail(nullptr){}
    void Print_Final_Answer() {
        //if (head == tail) return;
        AnsNode* temp = head;
        int r, c;
        while (temp != nullptr) {
            c = temp->col;
            r = temp->row;
            //cout << r << " " << c << endl;
            outFile << r << " " << c << endl;
            temp = temp->next; 
        }
    }
    void Clean() {
        AnsNode* temp = head;
        while (temp != nullptr) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }

    void CopyList(floor *start) {
        floor* temp = start;
        AnsNode* temp1;
        if ( head == nullptr && tail == nullptr ) {
            head = new AnsNode(start->f_row, start->f_col, nullptr);
            tail = head;
            temp = temp->prev;  // next
            temp1 = head;
            count++;
        }
        else temp1 = tail;
        //temp1 = temp1->next;
        while ( temp != nullptr ) {
            //AnsNode* new_node = new AnsNode(temp->f_row, temp->f_col, nullptr);
            //temp1 = new_node;
            if ( temp->prev == nullptr ) {
                tail = temp1; 
                break;
            }
            
            temp1->next = new AnsNode(temp->f_row, temp->f_col, nullptr);
            count++;
            temp1 = temp1->next;
            temp = temp->prev;   // next
        }
    }
};
class Clean_Robot{
    public:
        // initialize row, col, battery, map
        Clean_Robot(int m, int n, int B) {
            row = m;
            col = n;
            Battery = B;
            num = 0;
            map = new char*[m];
            map_check = new char*[m];
            visited = new bool*[m];
            for (int i=0; i<row; i++) {
                map[i] = new char[col];
                map_check[i] = new char[col];
                visited[i] = new bool[col];
            }
                
            
            char c;
            for (int i=0; i<row; i++){
                for (int j=0; j<col; j++) {
                    //cin >> c;
                    inFile >> c;
                    if (c == 'R') 
                        Recharge = new floor(i, j, 0);
                    else if (c == '0')
                        num++;
                    map[i][j] = c; 
                    map_check[i][j] = c;
                }
            }
        }
        
        // Print the map for debug
        void Print();

        // Initialize *Visited
        void Init_Visited();
        
        // for Breadth First Search
        floor* BFS_Path(floor *start, floor *end);
        
        // Store all dirty floor as pair<int, int>
        void Dirty_floor();

        // Print Stack content for debug
        void Print_stack();
        
        // Check whether is all clear ?
        bool IsClear();

        // for debug
        bool Check_All_Clean();

        // Access dirty (x, y)
        void GetPair(int &x, int &y);

        void traversal(floor* start);

        floor* Get_R_Pos() {
            floor* start = new floor(Recharge->f_row, Recharge->f_col, 0);
            return start;
        }

        // reverse singly linked-list 
        floor* reverse(floor *start);

        // check ever cleaned?
        bool Check_needed(int pos_row, int pos_col);
        
        void Clean(floor* start) {
            floor* temp = start;
            while (temp != nullptr) {
                temp = start;
                start = start->prev;
                delete temp;
            }
        }


    private:
        
        int row;     // map row
        int col;     // map col
        int Battery;  // robot battery
        int num;     // number of dirty place
        
        floor* Recharge;
        
        char **map;  
        char **map_check;
        bool **visited;

        stack<pair<int, int>> dirty;

};

void Clean_Robot::Print() {
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++)
            cout << map[i][j] << " ";
        cout << endl;
    }
}

void Clean_Robot::Init_Visited() {
    
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
            if ( map[i][j] == 'R') {
                visited[i][j] = true;        // charge station
            } else if ( map[i][j] == '0') {
                visited[i][j] = true;        // valid step
            } else { 
                visited[i][j] = false;        // obstacle
            }
        }
    }
}

floor* Clean_Robot::BFS_Path(floor *start, floor *end) {
    
    Init_Visited();
    visited[start->f_row][start->f_col] = false;
    
    queue<floor*> queue;
    queue.push(start);
    
    

    while ( !queue.empty() ) {
        floor *cur = queue.front();
        queue.pop();
        int r = cur->f_row;
        int c = cur->f_col;
        //cout << "r = " << r << " c = " << c << endl;
        if (r == end->f_row && c == end->f_col) return cur;  // return pointer which is backward to start
        //cout << "123" << endl;
        if ( r>=1 && visited[r-1][c] ) {               //向上走
            visited[r-1][c] = false;
            int up_distance_row = ( r-1 - Recharge->f_row );
            int up_distance_col = ( c   - Recharge->f_col );
            if ( up_distance_row<0 ) up_distance_row = 0 - up_distance_row ;
            if ( up_distance_col<0 ) up_distance_col = 0 - up_distance_col ;
            int up_total = up_distance_row + up_distance_col ;
            
            floor* step_up = new floor(r-1, c, up_total);
            
            step_up->prev = cur;
            queue.push(step_up);
            if (r-1 == end->f_row && c == end->f_col) return step_up;
        } 
        if ( c<=col-2 && visited[r][c+1]  ) {           //向右走
            visited[r][c+1] = false;
            int right_distance_row = ( r   - Recharge->f_row );
            int right_distance_col = ( c+1 - Recharge->f_col );
            if ( right_distance_row<0 ) right_distance_row = 0 - right_distance_row ;
            if ( right_distance_col<0 ) right_distance_col = 0 - right_distance_col ;
            int right_total = right_distance_row + right_distance_col ;

            floor* step_right = new floor(r, c+1, right_total);

            step_right->prev = cur;
            queue.push(step_right);
            if (r == end->f_row && c+1 == end->f_col) return step_right;
        } 
        if ( c>=1 && visited[r][c-1] ) {               //向左走
            visited[r][c-1] = false;
            int left_distance_row = ( r   - Recharge->f_row );
            int left_distance_col = ( c-1 - Recharge->f_col );
            if ( left_distance_row<0 ) left_distance_row = 0 - left_distance_row ;
            if ( left_distance_col<0 ) left_distance_col = 0 - left_distance_col ;
            int left_total = left_distance_row + left_distance_col ;

            floor* step_left = new floor(r, c-1, left_total);

            step_left->prev = cur;
            queue.push(step_left);
            if (r == end->f_row && c-1 == end->f_col) return step_left;
        }
        //cout << "456" << endl;
        if (  r<=row-2 && visited[r+1][c] ) {            //向下走
            visited[r+1][c] = false;
            int down_distance_row = ( r+1 - Recharge->f_row );
            int down_distance_col = ( c   - Recharge->f_col );
            if ( down_distance_row<0 ) down_distance_row = 0 - down_distance_row ;
            if ( down_distance_col<0 ) down_distance_col = 0 - down_distance_col ;
            int down_total = down_distance_row + down_distance_col ;

            floor* step_down = new floor(r+1, c, down_total);

            step_down->prev = cur;
            queue.push(step_down);
            if (r == end->f_row && c+1 == end->f_col) return step_down;
        } 
        //cout << "789" << endl;
    }
    cout << "Not found Target" << endl;
    return nullptr;
}

void Clean_Robot::Dirty_floor() {
    for (int i=row-1; i>=0; i--) {
        for (int j=col-1; j>=0; j--) {
            if (map[i][j] == '0') {
                pair<int, int> pos = make_pair(i, j);
                dirty.push(pos);
            }
        }
    }
}

void Clean_Robot::Print_stack(){
    
    while ( !dirty.isEmpty() ) {
        cout << dirty.Top().first << " " << dirty.Top().second << endl;
        dirty.pop();
    }
}

bool Clean_Robot::IsClear() {
    return dirty.isEmpty();
}

void Clean_Robot::GetPair(int &r, int &c) {
    r = dirty.Top().first;   // for row
    c = dirty.Top().second;  // for col
    num--;                   // dirty floor
    dirty.pop();
}
void Clean_Robot::traversal(floor* start) {    
    floor* temp = start;
    while (temp->prev != nullptr) {
        //cout << "(" << temp->f_row << "," << temp->f_col << ")" << endl;
        map_check[temp->f_row][temp->f_col] = '2';
        temp = temp->prev;
    }
    map_check[temp->f_row][temp->f_col] = '2';
    //cout << endl;
    return;
}

floor* Clean_Robot::reverse(floor *start) {
    floor *curr = start;
    floor *prev = nullptr;
    floor *next = nullptr;

    while (curr != nullptr) {
        next = curr->prev;
        curr->prev = prev;
        prev = curr;
        curr = next;
    }
    start = prev;
    return start;
}

bool Clean_Robot::Check_needed(int pos_row, int pos_col) {
    if (map_check[pos_row][pos_col] == '2') return false;
    else return true;
}

bool Clean_Robot::Check_All_Clean() {
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
            if (map_check[i][j] == '0') return false;
        }
    }
    return true;
}

int main() 
{
    //ifstream inFile("floor.data", ios::in);
    if (! inFile) {
        cout << "cannot open floor.data" << endl;
        return 1;
    }
    if (! outFile) {
        cout << "cannot open floor.output" << endl;
        return 1;
    }
    int m, n, battery;
    //cin >> m >> n >> battery;
    inFile >> m >> n >> battery;   


    Clean_Robot robot(m, n, battery);  // Initialize map
    
    robot.Dirty_floor();                // Store dirty floor as pair<int, int>
    
    floor *a;
    Ans Record;

    int pos_r, pos_c;                   

    while ( !robot.IsClear() ) {         
        
        robot.GetPair(pos_r, pos_c);    // access dirty pair<int, int>

        if ( !robot.Check_needed(pos_r, pos_c) ) continue;       

        //cout << "pos_r = " << pos_r << " pos_c = " << pos_c << endl;
        floor *start = robot.Get_R_Pos();
        floor *end = new floor(pos_r, pos_c, 0);  
        

        floor *path = robot.BFS_Path(start, end);    // create path from R to 0

        floor* forward = robot.reverse(path);
        robot.traversal(forward);
        Record.CopyList(forward);
        
        //Record.Print_Final_Answer();
        //cout << endl;
        floor* backward = robot.reverse(forward);
        robot.traversal(backward);
        Record.CopyList(backward);
        
        
        //Record.Print_Final_Answer();
        //cout << endl;
    }
    //if ( robot.Check_All_Clean()) cout << " All Clean !!!" << endl;
    //cout << count << endl;
    outFile << count << endl;
    Record.Print_Final_Answer();
    floor* R_Pos = robot.Get_R_Pos();
    //cout << R_Pos->f_row << " " << R_Pos->f_col << endl;
    outFile << R_Pos->f_row << " " << R_Pos->f_col << endl;
    return 0;    
}