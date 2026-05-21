


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <set>

using namespace std;


// first the code strcuter

typedef struct s_troll
{
    int id;
    int x;
    int y;
    int movement_speed;
    int harvest_power;
    int max_capacity;
    int carried;
    int carry_plum;
    int carry_lemon;
    int carry_apple;
    int carry_banana;
    int carry_wood;
} t_troll;

typedef struct s_tree
{
    string type;
    int x;
    int y;
    int amount;
} t_tree;

/* ================= UTIL ================= */

bool has_tree_at(int x, int y, const vector<t_tree>& trees)
{
    for (int i = 0; i < (int)trees.size(); i++)
        if (trees[i].x == x && trees[i].y == y)
            return true;
    return false;
}

/* BFS safe cell */
pair<int,int> find_nearest_safe_cell(
    int shack_x,
    int shack_y,
    const vector<t_tree>& trees,
    const vector<string>& grid,
    int width,
    int height)
{
    queue<pair<int,int>> q;
    set<pair<int,int>> visited;

    q.push({shack_x, shack_y});
    visited.insert({shack_x, shack_y});

    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

    while (!q.empty())
    {
        auto cur = q.front();
        q.pop();

        int x = cur.first;
        int y = cur.second;

        if (x >= 0 && y >= 0 && x < width && y < height)
        {
            char cell = grid[y][x];

            if (cell == '~' || cell == '#' || cell == '+')
                continue;

            if (!(x == shack_x && y == shack_y) &&
                !has_tree_at(x, y, trees))
                return {x, y};
        }

        for (int i = 0; i < 4; i++)
        {
            int nx = x + dirs[i][0];
            int ny = y + dirs[i][1];

            if (!visited.count({nx, ny}))
            {
                visited.insert({nx, ny});
                q.push({nx, ny});
            }
        }
    }

    return {shack_x, shack_y};
}

/* nearest tree */
void search_best_tree(
    const vector<t_tree>& trees,
    int x, int y,
    int& best_x, int& best_y,
    int& second_x, int& second_y)
{
    int best = 1e9, second = 1e9;
    best_x = best_y = second_x = second_y = -1;

    for (auto &t : trees)
    {
        if (t.amount <= 0) continue;

        int d = abs(x - t.x) + abs(y - t.y);

        if (d < best)
        {
            second = best;
            second_x = best_x;
            second_y = best_y;

            best = d;
            best_x = t.x;
            best_y = t.y;
        }
        else if (d < second)
        {
            second = d;
            second_x = t.x;
            second_y = t.y;
        }
    }
}

bool is_near_shack(int x, int y, int sx, int sy)
{
    return abs(x - sx) + abs(y - sy) == 1;
}

pair<int,int> find_tree_on_enemy_shack(
    const vector<t_tree>& trees,
    int ex,
    int ey)
{
    for (auto &t : trees)
        if (t.x == ex && t.y == ey)
            return {t.x, t.y};

    return {-1, -1};
}

/* ================= MAIN ================= */

int main()
{
    int width, height;
    cin >> width >> height;
    cin.ignore();

    int shack_x = -1, shack_y = -1;
    int enemy_shack_x = -1, enemy_shack_y = -1;

    vector<string> grid;

    for (int y = 0; y < height; y++)
    {
        string line;
        getline(cin, line);
        grid.push_back(line);

        for (int x = 0; x < width; x++)
        {
            if (line[x] == '0') { shack_x = x; shack_y = y; }
            if (line[x] == '1') { enemy_shack_x = x; enemy_shack_y = y; }
        }
    }

    int max_trainers = 5;
    int trainers_used = 0;

    /* GAME LOOP */
    while (1)
    {
        int my_plum, my_lemon, my_apple;
        for (int i = 0; i < 2; i++)
        {
            int plum, lemon, apple, banana, iron, wood;
            cin >> plum >> lemon >> apple >> banana >> iron >> wood;
            cin.ignore();

            if (i == 0)
            {
                my_plum = plum;
                my_lemon = lemon;
                my_apple = apple;
            }
        }

        int trees_count;
        cin >> trees_count;
        cin.ignore();

        vector<t_tree> trees;

        for (int i = 0; i < trees_count; i++)
        {
            string type;
            int x,y,size,health,fruits,cooldown;
            cin >> type >> x >> y >> size >> health >> fruits >> cooldown;
            cin.ignore();

            trees.push_back({type, x, y, fruits});
        }

        int trolls_count;
        cin >> trolls_count;
        cin.ignore();

        vector<t_troll> my_trolls;

        for (int i = 0; i < trolls_count; i++)
        {
            int id, player, x,y,ms,cap,hp,cp;
            int p,l,a,b,ir,wd;

            cin >> id >> player >> x >> y >> ms >> cap >> hp >> cp
                >> p >> l >> a >> b >> ir >> wd;
            cin.ignore();

            if (player == 0)
            {
                t_troll t;
                t.id = id;
                t.x = x;
                t.y = y;
                t.max_capacity = cap;
                t.carry_plum = p;
                t.carry_lemon = l;
                t.carry_apple = a;
                t.carry_banana = b;

                t.carried = p+l+a+b;

                my_trolls.push_back(t);
            }
        }

        string output = "";
        bool first_action = true;

        bool can_train =
            my_plum >= 2 &&
            my_lemon >= 2 &&
            my_apple >= 2 &&
            trainers_used < max_trainers;

        /* TRAIN ONCE PER TURN */
        if (can_train)
        {
            output += "TRAIN 1 1 1 0";
            trainers_used++;
            first_action = false;
        }

        for (int i = 0; i < (int)my_trolls.size(); i++)
        {
            t_troll me = my_trolls[i];

            if (!output.empty()) output += ";";

            bool full = me.carried >= me.max_capacity;
            bool near_shack = is_near_shack(me.x, me.y, shack_x, shack_y);

            /* ENEMY TREE PRIORITY */
            auto enemy_tree =
                find_tree_on_enemy_shack(trees, enemy_shack_x, enemy_shack_y);

            if (enemy_tree.first != -1)
            {
                if (me.x == enemy_tree.first && me.y == enemy_tree.second)
                {
                    output += "CHOP " + to_string(me.id);
                }
                else
                {
                    output += "MOVE " + to_string(me.id) + " " +
                              to_string(enemy_tree.first) + " " +
                              to_string(enemy_tree.second);
                }
                continue;
            }

            /* DROP */
            if (near_shack && me.carried > 0)
            {
                output += "DROP " + to_string(me.id);
                continue;
            }

            /* RETURN */
            if (full)
            {
                output += "MOVE " + to_string(me.id) + " " +
                          to_string(shack_x) + " " +
                          to_string(shack_y);
                continue;
            }

            /* TREE TARGET */
            int tx, ty, tx2, ty2;
            search_best_tree(trees, me.x, me.y, tx, ty, tx2, ty2);

            if (tx != -1)
            {
                if (me.x == tx && me.y == ty)
                    output += "HARVEST " + to_string(me.id);
                else
                    output += "MOVE " + to_string(me.id) + " " +
                              to_string(tx) + " " + to_string(ty);
                continue;
            }

            /* DEFAULT */
            output += "MOVE " + to_string(me.id) + " " +
                      to_string(shack_x) + " " + to_string(shack_y);
        }

        cout << output << endl;
    }
}