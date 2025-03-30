#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <random>
#include <algorithm>

using namespace std;

struct Car {
    int ID;
    pair<int, int> start;
    pair<int, int> end;
    string orientation;
    bool operator==(const Car& other) const {
        return ID == other.ID;
    }
};

struct Result {
    int x;
    int y;
    char move;
    int distance;
};


int main() {
    int W, H, N;
    cin >> W >> H >> N; // Wczytanie W, H i N

    // Wczytanie ciągu H wierszy
    vector<string> lines;
    string line;
    getline(cin, line); // Usunięcie znaku nowej linii z bufora

    for (int i = 0; i < H; ++i) {
        getline(cin, line); // Wczytanie kolejnego wiersza
        lines.push_back(line);
    }

    // WxH (szerokość, wysokość)

    vector<Car> cars;

    Car dummy = {-1,{H-1,0},{H-1,0},"."};
    cars.push_back(dummy);

    int id = 1;
    pair<int, int> s,e;

    // auto dziekana
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W ; ++j) { //
            if (lines[i][j] == 'o') {
                int k = 1;
                s = {i,j};
                e = {i,j};
                while (lines[i + k][j] == 'o'){
                    e = {i+k,j};
                    k++;
                }
                while (lines[i][j+k] == 'o'){
                    e = {i,j+k};
                    k++;
                }
                Car new_car;
                if(s.first != e.first){
                    new_car = {id, s, e,"|"};
                }
                else{
                    new_car = {id, s, e,"-"};
                }
                cars.push_back(new_car);
                id++;
                break;
            }
        }
        if (id == 2){
            break;
        }
    }

    // auta pionowe
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W ; ++j) {
            if (lines[i][j] == 'x') {
                s = {i,j};
                e = {i,j};
                if (lines[i+1][j] == 'y'){
                    e = {i+1,j};
                    if (lines[i+2][j] == 'z'){
                        e = {i+2,j};
                        if (lines[i+3][j] == 'w'){
                            e = {i+3,j};
                        }
                    }
                }
                Car new_car = {id, s, e,"|"};
                cars.push_back(new_car);
                id++;
            }
        }
    }

    // auta poziome
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W ; ++j) {
            if (lines[i][j] == 'a') {
                s = {i,j};
                e = {i,j};
                if (lines[i][j+1] == 'b'){
                    e = {i,j+1};
                    if (lines[i][j+2] == 'c'){
                        e = {i,j+2};
                        if (lines[i][j+3] == 'd'){
                            e = {i,j+3};
                        }
                    }
                }
                Car new_car = {id, s, e,"-"};
                cars.push_back(new_car);
                id++;
            }
        }
    }

    // mapa z ID aut
    int arr[H][W];
//    int **arr;
//    arr = new int*[H];
//    for (int i = 0; i < H; i++){
//        arr[i] = new int[W];
//    }

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W ; ++j){
            if (lines[i][j] == '#'){
                arr[i][j] = 0;
            }
            else{
                arr[i][j] = -1;
            }
        }
    }

    for (auto& car : cars) {
        id = car.ID;
        for(int i = car.start.first; i <= car.end.first; i++)
        {
            for(int j = car.start.second; j <= car.end.second; j++)
            {
                arr[i][j] = id;
            }
        }
    }

    // wyjście
    int x_out,y_out;
    for (int i = 0; i < H; i++){
        if (lines[i][W-1] == '.'){
            y_out = i;
            x_out = W-1;
        }
        if (lines[i][0] == '.'){
            y_out = i;
            x_out = 0;
        }
    }
    for (int i = 0; i < W; i++){
        if (lines[H-1][i] == '.'){
            y_out = H-1;
            x_out = i;
        }
        if (lines[0][i] == '.'){
            y_out = 0;
            x_out = i;
        }
    }

    // vector rozwiązań
    vector<Result> result;
    result.reserve(N+2);

    // kopiowanie

    int arr_c[H][W];

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W ; ++j){
            arr_c[i][j] = arr[i][j];
        }
    }

    vector<pair<int,int>> starts,ends;

    for (auto& car : cars) {
        starts.push_back(car.start);
        ends.push_back(car.end);
    }

    //
    int moves;

    random_device rd;
    mt19937 gen(rd());

    //
    int dean_column = cars[1].start.second;
    int dean_line = cars[1].start.first;
    int f;
    int move_l, how_far_b,how_far_f;
    int c_prev = 0;
    //


//    for (int i = 0; i < H; ++i) {
//        for (int j = 0; j < W ; ++j){
//            cout << arr[i][j] << "   ";
//        }
//        cout << endl;
//    }
//    cout << endl;
    //
    int flag = 0;
    while (flag == 0){

        // powrót do wartości podstawowych
        // mapa
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W ; ++j){
                arr[i][j] = arr_c[i][j];
            }
        }
        // auta
        for (int i = 1; i < cars.size(); i++) {
            cars[i].start = starts[i];
            cars[i].end = ends[i];
        }
        // czyszczenie result
        if (!result.empty()){
            result.clear();
        }
        // testowanie rozwiązań
        moves = 0;
        f = 0;

        while (moves < N) {
//            cout << moves << endl;
//            for (int i = 0; i < H; ++i) {
//                for (int j = 0; j < W ; ++j){
//                    cout << arr[i][j]<< " ";
//                }
//                cout << endl;
//            }
//            cout << endl;

            // czy dziekan może wyjechać
            // dziekan "|"
            if (y_out == H-1 || y_out == 0){
                // wyjście u góry
                if (y_out == 0){
                    for(int i = 0; i < cars[1].start.first; i++){
                        if (arr[i][dean_column] != -1){
                            f = 0;
                            break;
                        }
                        f = 1;
                    }

                    if (f == 1){
                        Result res = { cars[1].end.first, cars[1].end.second,'U',cars[1].start.first+1};
                        result.push_back(res);
                        flag = 1;
                        break;
                    }
                }
                    // wyjścieu z dołu
                else{
                    // pass
                }

            }
            // dziekan "-"
            if (x_out == W-1 || x_out == 0){
                // wyjście po lewej
                if( x_out == 0){
                    // pass
                }
                    // wyjście po prawej
                else{
                    for(int i = 1; i < W - cars[1].end.second; i++){
                        if (arr[dean_line][cars[1].end.second + i] != -1){
                            f = 0;
                            break;
                        }
                        f = 1;
                    }

                    if (f == 1){
                        Result res = { cars[1].end.first, cars[1].end.second,'R',10};
                        result.push_back(res);
                        flag = 1;
                        break;
                    }

                }

            }

            move_l = 0, how_far_b = 1, how_far_f = 1;

            // losowy samochód
            uniform_int_distribution<> distr1(1, cars.size() - 1);
            int c = distr1(gen);
            while (c == c_prev){
                c = distr1(gen);
            }
            c_prev = c;
            Car& chosen_car = cars[c];

            // jak daleko się poruszyć
            if (chosen_car.orientation == "|"){
                // backward
                while (arr[chosen_car.end.first+how_far_b][chosen_car.end.second] < 0){
                    how_far_b++;
                }
                how_far_b--;

                // forward
                while (arr[chosen_car.start.first - how_far_f][chosen_car.start.second] < 0){
                    how_far_f++;
                }
                how_far_f--;

                if(how_far_f == 0 && how_far_b == 0){
                    continue;
                }

                uniform_int_distribution<> distr3(-how_far_f, how_far_b);
                while(move_l == 0){
                    move_l = distr3(gen);
                }

                Result res{};
                if(move_l > 0){
                    // backward
                    res = {chosen_car.start.first,chosen_car.start.second,'D',move_l};

                }
                else{
                    // forward
                    res = {chosen_car.start.first,chosen_car.start.second,'U',-move_l};
                }
                // dodanie ruchu do rozwiązania
                moves++;
                result.push_back(res);

                //
                if(move_l > 0){
                    // backward
                    for (int i = 0;i < move_l;i++){
                        // poruszanie się
                        arr[chosen_car.start.first][chosen_car.start.second] = -1;
                        chosen_car.start.first++;
                        chosen_car.end.first++;
                        arr[chosen_car.end.first][chosen_car.end.second] = chosen_car.ID;
                    }
                }
                else{
                    // forward
                    for (int i = 0;i < -move_l;i++){
                        // poruszanie się
                        arr[chosen_car.end.first][chosen_car.end.second] = -1;
                        chosen_car.end.first--;
                        chosen_car.start.first--;
                        arr[chosen_car.start.first][chosen_car.start.second] = chosen_car.ID;

                    }
                }
            }
                // "-"
            else if (chosen_car.orientation == "-"){
                // forward
                while (arr[chosen_car.end.first][chosen_car.end.second+how_far_f] < 0){
                    how_far_f++;
                }
                how_far_f--;

                // backward
                while (arr[chosen_car.start.first][chosen_car.start.second-how_far_b] < 0){
                    how_far_b++;
                }
                how_far_b--;

                if(how_far_f == 0 && how_far_b == 0){
                    continue;
                }

                uniform_int_distribution<> distr4(-how_far_b, how_far_f);
                while(move_l == 0){
                    move_l = distr4(gen);
                }

                Result res{};
                if(move_l > 0){
                    // forward
                    res = {chosen_car.start.first,chosen_car.start.second,'R',move_l};
                }
                else{
                    // backward
                    res = {chosen_car.start.first,chosen_car.start.second,'L',-move_l};
                }

                // dodanie ruchu do rozwiązania
                moves++;
                result.push_back(res);
                //
                if(move_l > 0){
                    // forward
                    for (int i = 0;i < move_l;i++){
                        // poruszanie się
                        arr[chosen_car.start.first][chosen_car.start.second] = -1;
                        chosen_car.start.second++;
                        chosen_car.end.second++;
                        arr[chosen_car.end.first][chosen_car.end.second] = chosen_car.ID;
                    }
                }
                else{
                    // backward
                    for (int i = 0;i < -move_l;i++){
                        arr[chosen_car.end.first][chosen_car.end.second] = -1;
                        chosen_car.start.second--;
                        chosen_car.end.second--;
                        arr[chosen_car.start.first][chosen_car.start.second] = chosen_car.ID;
                    }
                }
            }
        }
    }

    // random moves dopuki moves <= N

//    for (int i = 0; i < H; ++i) {
//        for (int j = 0; j < W ; ++j){
//            cout << arr[i][j] << "   ";
//        }
//        cout << endl;
//    }

    // wyniki
    cout << result.size() << endl;
    for (auto& result_moves : result) {
        cout << result_moves.y << " " << result_moves.x << " " <<result_moves.move << " " << result_moves.distance << endl;
    }

    return 0;
}
