#include <iostream>
#include <utility>

#include "queue.h"
#include "stack.h"

using namespace std;

struct floor{
    int f_row;
    int f_col;
    int f_weight;
    floor *prev, *next;   // link path 
    floor(): f_row(0), f_col(0), f_weight(0), prev(nullptr), next(nullptr) {}
    floor(int r, int c, int w): f_row(r), f_col(c), f_weight(w), prev(nullptr), next(nullptr) {}
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
            for (int i=0; i<row; i++)
                map[i] = new char[col];
            
            char c;
            for (int i=0; i<row; i++){
                for (int j=0; j<col; j++) {
                    cin >> c;
                    if (c == 'R') 
                        Recharge = new floor(i, j, 0);
                    else if (c == '0')
                        num++;
                    map[i][j] = c; 
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
        
        // Check whether is clear ?
        bool IsClear();

        // Access dirty (x, y)
        void GetPair(int &x, int &y);

        floor* Get_R() {
            return Recharge;
        }

    private:
        
        int row;     // map row
        int col;     // map col
        int Battery;  // robot battery
        int num;     // number of dirty place
        
        floor* Recharge;
        
        char **map;  
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
    queue<floor*> queue;
    visited[start->f_row][start->f_col] = false;
    queue.push(start);

    while ( !queue.empty() ) {
        floor *cur = queue.front();
        queue.pop();
        
        int r = cur->f_row;
        int c = cur->f_col;

        if (r == end->f_row && c == end->f_col) return cur;  // return pointer which is backward to start

        if ( visited[r-1][c] && r>=1 ) {               //向上走
            int up_distance_row = ( r-1 - Recharge->f_row );
            int up_distance_col = ( c   - Recharge->f_col );
            if ( up_distance_row<0 ) up_distance_row = 0 - up_distance_row ;
            if ( up_distance_col<0 ) up_distance_col = 0 - up_distance_col ;
            int up_total = up_distance_row + up_distance_col ;

            floor* step_up = new floor(r-1, c, up_total);

            step_up->prev = cur;
            cur->next = step_up;
            queue.push(step_up);
        } 
        if ( visited[r][c+1] && c<=col-2 ) {           //向右走
            int right_distance_row = ( r   - Recharge->f_row );
            int right_distance_col = ( c+1 - Recharge->f_col );
            if ( right_distance_row<0 ) right_distance_row = 0 - right_distance_row ;
            if ( right_distance_col<0 ) right_distance_col = 0 - right_distance_col ;
            int right_total = right_distance_row + right_distance_col ;

            floor* step_right = new floor(r, c+1, right_total);

            step_right->prev = cur;
            cur->next = step_right;
            queue.push(step_right);
        } 
        if ( visited[r][c-1] && c>=1 ) {               //向左走
            int left_distance_row = ( r   - Recharge->f_row );
            int left_distance_col = ( c-1 - Recharge->f_col );
            if ( left_distance_row<0 ) left_distance_row = 0 - left_distance_row ;
            if ( left_distance_col<0 ) left_distance_col = 0 - left_distance_col ;
            int left_total = left_distance_row + left_distance_col ;

            floor* step_left = new floor(r, c+1, left_total);

            step_left->prev = cur;
            cur->next = step_left;
            queue.push(step_left);
        }
        if ( visited[r+1][c] && r<=row-2) {            //向下走
            int down_distance_row = ( r+1 - Recharge->f_row );
            int down_distance_col = ( c   - Recharge->f_col );
            if ( down_distance_row<0 ) down_distance_row = 0 - down_distance_row ;
            if ( down_distance_col<0 ) down_distance_col = 0 - down_distance_col ;
            int down_total = down_distance_row + down_distance_col ;

            floor* step_down = new floor(r, c+1, down_total);

            step_down->prev = cur;
            cur->next = step_down;
            queue.push(step_down);
        } 
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
    dirty.pop();
}
int main() 
{
    int m, n, battery;
    cin >> m >> n >> battery;

    Clean_Robot robot(m, n, battery);  // Initialize map

    robot.Dirty_floor();                // Store dirty floor as pair<int, int>

    int pos_r, pos_c;                   

    /*while ( !robot.IsClear() ) {         
        
        robot.GetPair(pos_r, pos_c);    // access dirty place
        
        floor *start = robot.Get_R();
        floor *end = new floor(pos_r, pos_c, 0);  
        
        floor *path = robot.BFS_Path(start, end);    // create path from R to 0 
    }*/

    

    return 0;    
}

