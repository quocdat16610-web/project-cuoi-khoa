# TÀI LIỆU PHÂN TÍCH – THIẾT KẾ GAME SNAKE C++

## 1. Giới thiệu

Đây là tài liệu phân tích và thiết kế chương trình **Game Snake (Rắn săn mồi)** được xây dựng bằng C++ và chạy trên môi trường console Windows.

Chương trình cho phép người chơi điều khiển con rắn bằng các phím **W, A, S, D**, di chuyển trong một khung chơi có kích thước cố định. Mục tiêu của trò chơi là điều khiển đầu rắn ăn thức ăn để tăng điểm và tăng chiều dài. Trò chơi kết thúc khi rắn va vào biên hoặc tự va vào thân.

Chương trình sử dụng các thư viện và kỹ thuật chính:

- `iostream`: nhập/xuất dữ liệu.
- `vector`: lưu các phần tử của thân rắn.
- `conio.h`: đọc phím trực tiếp bằng `_kbhit()` và `_getch()`.
- `windows.h`: điều khiển vị trí con trỏ console và tạo độ trễ.
- `cstdlib`, `ctime`: tạo số ngẫu nhiên cho vị trí thức ăn.
- `struct`: biểu diễn một tọa độ trên màn hình.
- `enum`: biểu diễn hướng di chuyển.

---

# 2. Phân tích yêu cầu

## 2.1. Yêu cầu chức năng

Chương trình cần thực hiện các chức năng sau:

1. Khởi tạo trò chơi.
2. Tạo con rắn ban đầu gồm 3 phần tử.
3. Tạo thức ăn ở một vị trí ngẫu nhiên.
4. Không cho thức ăn xuất hiện trùng với thân rắn.
5. Hiển thị bản đồ trò chơi.
6. Hiển thị đầu rắn, thân rắn và thức ăn bằng các ký tự khác nhau.
7. Cho phép người chơi điều khiển bằng:
   - `W`: đi lên.
   - `S`: đi xuống.
   - `A`: sang trái.
   - `D`: sang phải.
8. Không cho rắn quay ngược trực tiếp về phía thân.
9. Khi rắn ăn thức ăn:
   - Tăng điểm.
   - Tạo thức ăn mới.
   - Tăng chiều dài rắn.
10. Khi rắn đâm vào biên:
    - Kết thúc trò chơi.
11. Khi đầu rắn va vào thân:
    - Kết thúc trò chơi.
12. Cho phép nhấn `ESC` để kết thúc trò chơi.
13. Hiển thị màn hình `GAME OVER` và điểm số sau khi trò chơi kết thúc.

## 2.2. Yêu cầu phi chức năng

- Chương trình chạy trên console Windows.
- Giao diện đơn giản, trực quan.
- Trò chơi phản hồi phím trong khi đang chạy.
- Vị trí thức ăn được tạo ngẫu nhiên.
- Màn hình được cập nhật liên tục mà không cần in lại toàn bộ console theo cách cuộn dòng.

---

# 3. Phân tích dữ liệu

## 3.1. Kích thước bản đồ

Chương trình định nghĩa:

```cpp
const int WIDTH = 40;
const int HEIGHT = 20;
```

Trong đó:

- `WIDTH = 40`: chiều rộng vùng chơi.
- `HEIGHT = 20`: chiều cao vùng chơi.

Bản đồ thực tế được bao quanh bởi một đường viền nên số cột hiển thị có cả phần biên.

---

# 4. Thiết kế dữ liệu

## 4.1. Kiểu dữ liệu `Direction`

```cpp
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
```

`Direction` là kiểu liệt kê dùng để biểu diễn hướng di chuyển của rắn.

Có 4 trạng thái:

| Giá trị | Ý nghĩa |
|---|---|
| `UP` | Đi lên |
| `DOWN` | Đi xuống |
| `LEFT` | Đi sang trái |
| `RIGHT` | Đi sang phải |

Hướng ban đầu:

```cpp
Direction dir = RIGHT;
```

Do đó khi bắt đầu game, rắn di chuyển sang phải.

---

## 4.2. Cấu trúc `Point`

```cpp
struct Point
{
    int x;
    int y;
};
```

`Point` biểu diễn một vị trí trên bản đồ.

- `x`: tọa độ theo chiều ngang.
- `y`: tọa độ theo chiều dọc.

Ví dụ:

```text
Point p = {10, 5};
```

có nghĩa là điểm `p` nằm tại:

```text
x = 10
y = 5
```

---

## 4.3. Dữ liệu của con rắn

```cpp
vector<Point> snake;
```

`snake` là một `vector` chứa toàn bộ các phần tử của con rắn.

Quy ước:

```text
snake[0]      -> đầu rắn
snake[1]      -> thân
snake[2]      -> thân
...
snake.back()  -> đuôi
```

Ví dụ:

```text
snake = [
    {20,10},
    {19,10},
    {18,10}
]
```

có thể biểu diễn:

```text
OOOO
^
Đầu
```

Trong chương trình, đầu rắn được vẽ bằng `O`, các phần thân bằng `o`.

---

## 4.4. Dữ liệu thức ăn

```cpp
Point food;
```

`food` lưu tọa độ của thức ăn.

Thức ăn được hiển thị bằng ký tự:

```text
@
```

---

## 4.5. Trạng thái game

```cpp
bool gameOver = false;
```

Biến này xác định trò chơi còn tiếp tục hay không.

- `false`: game đang chạy.
- `true`: game kết thúc.

---

## 4.6. Điểm số

```cpp
int score = 0;
```

Mỗi lần ăn thức ăn:

```cpp
score += 10;
```

Do đó mỗi thức ăn tương ứng với 10 điểm.

---

# 5. Thiết kế tổng thể chương trình

Chương trình được chia thành các chức năng chính:

```text
main()
  |
  +-- Khởi tạo random
  |
  +-- Ẩn con trỏ console
  |
  +-- setup()
  |     |
  |     +-- Khởi tạo rắn
  |     +-- Khởi tạo hướng
  |     +-- Reset điểm
  |     +-- Reset trạng thái game
  |     +-- spawnFood()
  |
  +-- draw()
  |
  +-- Vòng lặp game
        |
        +-- input()
        |
        +-- update()
        |
        +-- draw()
        |
        +-- Sleep(100)
  |
  +-- Hiển thị GAME OVER
  |
  +-- Chờ phím
```

Đây là mô hình vòng lặp game cơ bản:

```text
Input → Update → Draw → Delay
  ↑                       |
  └───────────────────────┘
```

---

# 6. Phân tích chi tiết từng hàm

## 6.1. Hàm `gotoXY()`

```cpp
void gotoXY(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        coord
    );
}
```

### Mục đích

Đưa con trỏ console đến vị trí `(x, y)`.

### Các thành phần

```cpp
COORD coord;
```

`COORD` là kiểu dữ liệu của Windows dùng để biểu diễn tọa độ trong console.

```cpp
coord.X = x;
coord.Y = y;
```

Thiết lập vị trí con trỏ.

```cpp
GetStdHandle(STD_OUTPUT_HANDLE)
```

Lấy handle của cửa sổ console hiện tại.

```cpp
SetConsoleCursorPosition(...)
```

Đưa con trỏ đến tọa độ mong muốn.

### Vai trò trong game

Hàm này giúp chương trình vẽ lại màn hình từ vị trí đầu console thay vì liên tục in xuống các dòng mới.

---

# 7. Hàm `spawnFood()`

```cpp
void spawnFood()
```

## Mục đích

Tạo một vị trí thức ăn ngẫu nhiên và đảm bảo thức ăn không nằm trên thân rắn.

## Thuật toán

```text
Bắt đầu
   |
   v
Sinh x ngẫu nhiên
   |
Sinh y ngẫu nhiên
   |
Kiểm tra vị trí có trùng rắn?
   |
  / \
Có   Không
 |      |
Sinh lại  Đặt thức ăn
 |      |
 └──<───┘
```

### Sinh tọa độ

```cpp
food.x = rand() % WIDTH;
food.y = rand() % HEIGHT;
```

Do đó:

```text
0 <= food.x < WIDTH
0 <= food.y < HEIGHT
```

### Kiểm tra trùng

```cpp
for (Point p : snake)
{
    if (p.x == food.x && p.y == food.y)
    {
        trung = true;
        break;
    }
}
```

Chương trình duyệt qua từng phần tử của rắn.

Nếu có một phần tử có cùng `x` và `y` với thức ăn thì vị trí đó không hợp lệ.

### Ý nghĩa

Điều kiện:

```cpp
if (!trung)
    break;
```

chỉ thoát vòng `while` khi thức ăn được đặt ở vị trí không trùng với rắn.

---

# 8. Hàm `setup()`

```cpp
void setup()
```

## Mục đích

Đưa game về trạng thái ban đầu.

### Xóa rắn cũ

```cpp
snake.clear();
```

### Tạo rắn gồm 3 phần tử

```cpp
snake.push_back({WIDTH / 2, HEIGHT / 2});
snake.push_back({WIDTH / 2 - 1, HEIGHT / 2});
snake.push_back({WIDTH / 2 - 2, HEIGHT / 2});
```

Rắn được đặt gần giữa bản đồ.

Ví dụ với:

```text
WIDTH = 40
HEIGHT = 20
```

thì đầu rắn bắt đầu gần:

```text
(20, 10)
```

và thân nằm phía bên trái.

### Reset trạng thái

```cpp
dir = RIGHT;
score = 0;
gameOver = false;
```

### Tạo thức ăn

```cpp
spawnFood();
```

---

# 9. Hàm `draw()`

```cpp
void draw()
```

## Mục đích

Vẽ toàn bộ trạng thái hiện tại của game lên console.

## Quy trình

### Bước 1: Đưa con trỏ về đầu

```cpp
gotoXY(0, 0);
```

### Bước 2: Vẽ cạnh trên

```cpp
for (int i = 0; i < WIDTH + 2; i++)
    cout << "#";
```

### Bước 3: Vẽ từng dòng

Chương trình duyệt:

```cpp
for (int y = 0; y < HEIGHT; y++)
```

và trong mỗi dòng duyệt:

```cpp
for (int x = 0; x < WIDTH; x++)
```

Như vậy chương trình xét từng ô `(x, y)` trên bản đồ.

---

## 9.1. Quy tắc vẽ rắn

Chương trình duyệt qua:

```cpp
for (int i = 0; i < snake.size(); i++)
```

Nếu:

```cpp
snake[i].x == x &&
snake[i].y == y
```

thì ô hiện tại thuộc về rắn.

Nếu:

```cpp
i == 0
```

thì đây là đầu:

```text
O
```

Ngược lại là thân:

```text
o
```

---

## 9.2. Quy tắc vẽ thức ăn

Nếu ô hiện tại không phải rắn:

```cpp
!printed
```

và tọa độ trùng thức ăn:

```cpp
food.x == x && food.y == y
```

thì in:

```text
@
```

---

## 9.3. Ô trống

Nếu ô không phải rắn và không phải thức ăn:

```cpp
cout << " ";
```

---

## 9.4. Vẽ điểm

Cuối bản đồ:

```cpp
cout << "\nScore: " << score << "     ";
```

Điểm hiện tại được hiển thị cho người chơi.

---

# 10. Hàm `input()`

```cpp
void input()
```

## Mục đích

Đọc phím người chơi nhấn và thay đổi hướng di chuyển.

Chương trình sử dụng:

```cpp
_kbhit()
```

để kiểm tra xem có phím nào được nhấn hay không.

Sau đó:

```cpp
_getch()
```

đọc trực tiếp ký tự.

---

## 10.1. Điều khiển

| Phím | Hướng |
|---|---|
| `W` / `w` | `UP` |
| `S` / `s` | `DOWN` |
| `A` / `a` | `LEFT` |
| `D` / `d` | `RIGHT` |
| `ESC` | Kết thúc game |

---

## 10.2. Chống quay đầu

Ví dụ:

```cpp
if ((key == 'w' || key == 'W') && dir != DOWN)
```

Nếu rắn đang đi xuống thì không cho đổi trực tiếp lên trên.

Tương tự:

```text
UP    ↔ DOWN
LEFT  ↔ RIGHT
```

Việc này tránh trường hợp đầu rắn lập tức quay ngược vào thân.

---

# 11. Hàm `update()`

```cpp
void update()
```

Đây là hàm xử lý logic chính của game.

## 11.1. Lấy vị trí đầu hiện tại

```cpp
Point newHead = snake[0];
```

Tạo một vị trí đầu mới dựa trên đầu hiện tại.

---

## 11.2. Tính vị trí đầu mới

### Đi lên

```cpp
newHead.y--;
```

### Đi xuống

```cpp
newHead.y++;
```

### Đi trái

```cpp
newHead.x--;
```

### Đi phải

```cpp
newHead.x++;
```

Có thể biểu diễn:

```text
UP       (x, y-1)

LEFT (x-1,y)  (x,y)  RIGHT (x+1,y)

DOWN      (x,y+1)
```

---

# 12. Kiểm tra va chạm biên

```cpp
if (newHead.x < 0 ||
    newHead.x >= WIDTH ||
    newHead.y < 0 ||
    newHead.y >= HEIGHT)
{
    gameOver = true;
    return;
}
```

Điều kiện này kiểm tra đầu mới có nằm ngoài vùng chơi hay không.

Nếu:

```text
x < 0
```

rắn đi ra bên trái.

Nếu:

```text
x >= WIDTH
```

rắn đi ra bên phải.

Nếu:

```text
y < 0
```

rắn đi lên quá biên.

Nếu:

```text
y >= HEIGHT
```

rắn đi xuống quá biên.

Khi xảy ra một trong các trường hợp trên:

```cpp
gameOver = true;
```

và hàm `update()` kết thúc ngay bằng:

```cpp
return;
```

---

# 13. Kiểm tra rắn tự cắn thân

```cpp
for (int i = 0; i < snake.size(); i++)
{
    if (newHead.x == snake[i].x &&
        newHead.y == snake[i].y)
    {
        gameOver = true;
        return;
    }
}
```

Chương trình duyệt toàn bộ thân rắn.

Nếu đầu mới trùng tọa độ với một phần tử của rắn thì game kết thúc.

---

# 14. Cơ chế di chuyển của rắn

Đây là phần quan trọng nhất trong thuật toán.

Sau khi tính được đầu mới:

```cpp
snake.insert(snake.begin(), newHead);
```

đầu mới được chèn vào đầu `vector`.

Ví dụ ban đầu:

```text
[ A, B, C ]
```

Sau khi đầu mới là `X`:

```text
[ X, A, B, C ]
```

Nếu rắn **không ăn thức ăn**, chương trình xóa phần tử cuối:

```cpp
snake.pop_back();
```

Kết quả:

```text
[ X, A, B ]
```

Như vậy rắn di chuyển nhưng vẫn giữ nguyên chiều dài.

---

# 15. Cơ chế ăn thức ăn

Sau khi thêm đầu mới, chương trình kiểm tra:

```cpp
if (newHead.x == food.x &&
    newHead.y == food.y)
```

Nếu đầu rắn trùng vị trí thức ăn:

```cpp
score += 10;
spawnFood();
```

Điểm tăng thêm 10 và tạo thức ăn mới.

Điểm đặc biệt là chương trình **không gọi `pop_back()`** trong trường hợp ăn thức ăn.

Vì vậy:

```text
Trước:
[ A, B, C ]

Thêm đầu:
[ X, A, B, C ]

Ăn thức ăn:
không xóa C

Sau:
[ X, A, B, C ]
```

Chiều dài rắn tăng thêm 1.

---

# 16. Hàm `main()`

```cpp
int main()
```

Đây là hàm điều khiển toàn bộ chương trình.

## 16.1. Khởi tạo bộ sinh số ngẫu nhiên

```cpp
srand(time(NULL));
```

Dùng thời gian hiện tại làm seed cho `rand()`.

Mục đích là giúp vị trí thức ăn thay đổi giữa các lần chạy.

---

## 16.2. Ẩn con trỏ console

Chương trình lấy thông tin console:

```cpp
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
```

Sau đó:

```cpp
GetConsoleCursorInfo(hConsole, &cursorInfo);
cursorInfo.bVisible = false;
SetConsoleCursorInfo(hConsole, &cursorInfo);
```

để ẩn con trỏ.

Điều này giúp giao diện game gọn hơn khi liên tục vẽ lại màn hình.

---

# 17. Vòng lặp game

Sau khi:

```cpp
setup();
draw();
```

chương trình bắt đầu vòng lặp:

```cpp
while (!gameOver)
{
    input();
    update();
    draw();
    Sleep(100);
}
```

Đây chính là **Game Loop**.

## Thứ tự xử lý

```text
       ┌───────────────┐
       │     INPUT     │
       │  Đọc bàn phím │
       └───────┬───────┘
               ↓
       ┌───────────────┐
       │    UPDATE     │
       │ Cập nhật logic│
       └───────┬───────┘
               ↓
       ┌───────────────┐
       │     DRAW      │
       │ Vẽ trạng thái │
       └───────┬───────┘
               ↓
       ┌───────────────┐
       │  SLEEP(100)   │
       │   Chờ 100ms   │
       └───────┬───────┘
               │
               └──────────→ Lặp lại
```

---

# 18. Tốc độ game

```cpp
Sleep(100);
```

`Sleep(100)` làm chương trình tạm dừng khoảng 100 mili-giây sau mỗi vòng lặp.

Có thể hiểu gần đúng là:

```text
1000 ms / 100 ms = 10
```

nên game xử lý khoảng 10 lần mỗi giây, tùy thời gian thực hiện các thao tác khác.

Nếu giảm:

```cpp
Sleep(50);
```

game sẽ nhanh hơn.

Nếu tăng:

```cpp
Sleep(200);
```

game sẽ chậm hơn.

---

# 19. Kết thúc trò chơi

Khi:

```cpp
gameOver == true
```

vòng:

```cpp
while (!gameOver)
```

dừng lại.

Sau đó chương trình đưa con trỏ xuống dưới bản đồ:

```cpp
gotoXY(0, HEIGHT + 3);
```

và hiển thị:

```text
=========================
       GAME OVER!
       Score: ...
=========================

Nhan phim bat ky de thoat...
```

Cuối cùng:

```cpp
_getch();
return 0;
```

chờ người chơi nhấn một phím trước khi thoát.

---

# 20. Thiết kế thuật toán tổng quát

## 20.1. Thuật toán khởi tạo

```text
START
  |
  ↓
Khởi tạo random
  |
  ↓
Ẩn cursor
  |
  ↓
Xóa rắn cũ
  |
  ↓
Tạo rắn 3 phần tử
  |
  ↓
Đặt hướng RIGHT
  |
  ↓
Score = 0
  |
  ↓
GameOver = false
  |
  ↓
Sinh thức ăn
```

---

## 20.2. Thuật toán xử lý một lượt game

```text
Đọc phím
   |
   ↓
Có phím điều khiển?
   |
  Có
   ↓
Cập nhật hướng
   |
   ↓
Tính đầu mới
   |
   ↓
Đụng biên?
  / \
Có   Không
 |      |
Game     ↓
Over   Đụng thân?
        /     \
       Có     Không
       |         |
    Game Over    ↓
             Thêm đầu mới
                  |
                  ↓
             Ăn thức ăn?
               /    \
             Có      Không
             |          |
        Tăng điểm      Xóa đuôi
        Sinh thức ăn       |
             |             |
             └──────┬──────┘
                    ↓
                  Vẽ game
                    |
                    ↓
                 Chờ 100ms
                    |
                    ↓
                 Lặp lại
```

---

# 21. Phân tích độ phức tạp

Gọi `L` là độ dài hiện tại của rắn.

## `spawnFood()`

Mỗi lần tạo thức ăn, chương trình duyệt qua toàn bộ rắn để kiểm tra trùng.

Độ phức tạp một lần kiểm tra:

```text
O(L)
```

## `draw()`

Trong mỗi ô của bản đồ, chương trình có thể duyệt qua toàn bộ rắn để xác định ô đó có thuộc rắn hay không.

Số ô bản đồ là:

```text
WIDTH × HEIGHT
```

Do đó độ phức tạp gần:

```text
O(WIDTH × HEIGHT × L)
```

Với:

```text
WIDTH = 40
HEIGHT = 20
```

thì kích thước bản đồ cố định và nhỏ.

## `update()`

Kiểm tra va chạm với thân rắn bằng cách duyệt `snake`:

```text
O(L)
```

Thao tác:

```cpp
snake.insert(snake.begin(), newHead);
```

trên `vector` có thể cần dịch chuyển các phần tử, nên về mặt lý thuyết là:

```text
O(L)
```

Tương tự:

```cpp
snake.pop_back();
```

là:

```text
O(1)
```

---

# 22. Mô hình dữ liệu của chương trình

Có thể biểu diễn các thành phần chính như sau:

```text
                GAME
                 |
       ┌─────────┼─────────┐
       |         |         |
     Snake      Food      Score
       |         |         |
 vector<Point> Point      int
       |
   ┌───┴────┐
   |        |
Head      Body
Point    Point...
```

---

# 23. Thiết kế giao diện

Giao diện console sử dụng các ký tự:

| Ký tự | Đối tượng |
|---|---|
| `#` | Biên bản đồ |
| `O` | Đầu rắn |
| `o` | Thân rắn |
| `@` | Thức ăn |
| khoảng trắng | Ô trống |

Ví dụ:

```text
##########################################
#                                        #
#                                        #
#              @                         #
#                                        #
#                   oooO                 #
#                                        #
#                                        #
##########################################
Score: 10
```

---

# 24. Luồng dữ liệu

Các biến toàn cục chính:

```text
snake ────────→ trạng thái vị trí rắn
food ─────────→ vị trí thức ăn
dir ──────────→ hướng di chuyển
score ────────→ điểm số
gameOver ─────→ trạng thái kết thúc
```

Các hàm tương tác với dữ liệu:

```text
setup()
   ↓
snake, food, dir, score, gameOver

input()
   ↓
dir, gameOver

update()
   ↓
snake, food, score, gameOver

draw()
   ↓
snake, food, score
```

---

# 25. Thiết kế module

| Module | Chức năng |
|---|---|
| `gotoXY()` | Điều khiển vị trí con trỏ |
| `spawnFood()` | Tạo thức ăn |
| `setup()` | Khởi tạo game |
| `draw()` | Vẽ giao diện |
| `input()` | Đọc phím |
| `update()` | Cập nhật logic |
| `main()` | Điều phối toàn bộ chương trình |

Mối quan hệ:

```text
                    main()
                      |
          ┌───────────┼───────────┐
          ↓           ↓           ↓
       setup()      draw()     Game Loop
                                  |
                         ┌────────┼────────┐
                         ↓        ↓        ↓
                      input()  update()  draw()
                                  |
                                  ↓
                             spawnFood()
```

---

# 26. Các trạng thái của game

Chương trình có thể được hiểu theo các trạng thái:

```text
       ┌───────────────┐
       │   KHỞI TẠO    │
       └───────┬───────┘
               ↓
       ┌───────────────┐
       │  ĐANG CHƠI    │
       └───────┬───────┘
          ┌────┴────┐
          │         │
       Ăn mồi     Va chạm/
          │       ESC
          │         │
          ↓         ↓
       Tăng dài   GAME OVER
          │
          └─────────→ ĐANG CHƠI
```

---

# 27. Các điểm cần chú ý khi triển khai

## 27.1. Phụ thuộc Windows

Code sử dụng:

```cpp
#include <windows.h>
#include <conio.h>
```

và các hàm:

```cpp
SetConsoleCursorPosition()
GetStdHandle()
Sleep()
_kbhit()
_getch()
```

Do đó chương trình được thiết kế cho môi trường **Windows Console**.

## 27.2. Không sử dụng thư viện đồ họa

Game không dùng:

- SDL
- SFML
- OpenGL
- DirectX

Toàn bộ giao diện được vẽ bằng ký tự console.

## 27.3. Quản lý thân rắn

`vector<Point>` là cấu trúc dữ liệu trung tâm để quản lý rắn.

Mỗi lần di chuyển:

```text
Thêm đầu mới
+
Xóa đuôi nếu không ăn
```

Đây là cơ chế cốt lõi tạo ra chuyển động của Snake.

---

# 28. Kiểm thử chương trình

## Test 1 – Khởi động

**Input:** Chạy chương trình.

**Kỳ vọng:**

- Rắn xuất hiện ở gần giữa bản đồ.
- Rắn có 3 phần tử.
- Hướng ban đầu là sang phải.
- Có thức ăn.
- Điểm bằng 0.

---

## Test 2 – Di chuyển

**Input:** Nhấn `W`.

**Kỳ vọng:**

- Rắn chuyển hướng lên trên nếu hướng hiện tại không phải `DOWN`.

---

## Test 3 – Chống quay đầu

**Input:**

1. Rắn đang đi phải.
2. Nhấn `A`.

**Kỳ vọng:**

- Rắn không chuyển sang trái ngay vì đây là hướng ngược lại.

---

## Test 4 – Ăn thức ăn

**Điều kiện:** Đầu rắn đi vào vị trí thức ăn.

**Kỳ vọng:**

```text
score += 10
```

Rắn tăng chiều dài thêm 1 và thức ăn được sinh ở vị trí mới.

---

## Test 5 – Đụng biên

**Input:** Cho rắn đi thẳng đến mép bản đồ.

**Kỳ vọng:**

```text
gameOver = true
```

Màn hình `GAME OVER` xuất hiện.

---

## Test 6 – Tự cắn

**Input:** Điều khiển rắn quay vòng sao cho đầu chạm vào thân.

**Kỳ vọng:**

- `gameOver = true`.
- Trò chơi kết thúc.

---

## Test 7 – ESC

**Input:** Nhấn `ESC`.

**Kỳ vọng:**

- `gameOver = true`.
- Thoát vòng lặp game.
- Hiển thị màn hình kết thúc.

---

# 29. Đánh giá thiết kế hiện tại

## Ưu điểm

- Cấu trúc chương trình tương đối rõ ràng.
- Các chức năng được chia thành các hàm riêng.
- `vector<Point>` phù hợp để biểu diễn thân rắn.
- `enum Direction` giúp việc quản lý hướng dễ hiểu.
- Có kiểm tra thức ăn không trùng với rắn.
- Có xử lý va chạm với biên.
- Có xử lý va chạm với thân.
- Có cơ chế chống quay đầu.
- Có game loop rõ ràng.
- Có điểm số và cơ chế tăng chiều dài.

## Hạn chế

- Chương trình phụ thuộc vào Windows.
- Việc vẽ bằng console chưa được tối ưu về mặt cấu trúc dữ liệu.
- `draw()` có thể phải duyệt nhiều phần tử để xác định từng ô thuộc về rắn.
- `snake.insert(snake.begin(), ...)` trên `vector` có chi phí dịch chuyển phần tử.
- Các biến trạng thái chính đang được khai báo toàn cục.
- Chưa có menu bắt đầu, tạm dừng hoặc chọn độ khó.
- Chưa có lưu điểm cao.
- Chưa có nhiều cấp độ tốc độ.

---

# 30. Hướng phát triển

Nếu tiếp tục phát triển dự án, có thể bổ sung:

1. **Tăng độ khó**
   - Tăng tốc độ theo điểm.
   - Thêm nhiều mức độ.

2. **Menu**
   - Start Game.
   - Instructions.
   - Exit.

3. **High Score**
   - Lưu điểm cao nhất ra file.

4. **Các loại thức ăn**
   - Thức ăn thường.
   - Thức ăn đặc biệt.
   - Thức ăn tăng điểm.

5. **Chướng ngại vật**
   - Thêm tường hoặc vật cản.

6. **Tạm dừng**
   - Nhấn một phím để Pause/Resume.

7. **Cải thiện kiến trúc**
   - Đóng gói trạng thái game vào `class Game`.
   - Tách input, logic và rendering thành các module riêng.

8. **Cải thiện cấu trúc dữ liệu**
   - Có thể cân nhắc `deque<Point>` cho thao tác thêm đầu và xóa đuôi thường xuyên.

---

# 31. Pseudocode tổng quát

```text
PROGRAM SNAKE

Khởi tạo WIDTH = 40
Khởi tạo HEIGHT = 20

Tạo Snake
Tạo Food
Tạo Direction
Tạo Score
Tạo GameOver

FUNCTION setup:
    Xóa snake
    Tạo 3 đoạn snake ở giữa màn hình
    Direction = RIGHT
    Score = 0
    GameOver = false
    spawnFood

FUNCTION spawnFood:
    Lặp:
        Tạo tọa độ food ngẫu nhiên
        Kiểm tra food có trùng snake không
    Cho đến khi vị trí hợp lệ

FUNCTION input:
    Nếu có phím:
        Đọc phím
        Nếu W và không đi DOWN:
            Direction = UP
        Nếu S và không đi UP:
            Direction = DOWN
        Nếu A và không đi RIGHT:
            Direction = LEFT
        Nếu D và không đi LEFT:
            Direction = RIGHT
        Nếu ESC:
            GameOver = true

FUNCTION update:
    Lấy đầu hiện tại
    Tạo newHead dựa trên Direction

    Nếu newHead nằm ngoài bản đồ:
        GameOver = true
        return

    Nếu newHead trùng thân:
        GameOver = true
        return

    Thêm newHead vào đầu snake

    Nếu newHead trùng food:
        Score += 10
        spawnFood
    Ngược lại:
        Xóa đuôi snake

FUNCTION draw:
    Đưa cursor về đầu console
    Vẽ biên
    Với mỗi ô:
        Nếu là đầu snake:
            Vẽ O
        Nếu là thân snake:
            Vẽ o
        Nếu là food:
            Vẽ @
        Nếu không:
            Vẽ khoảng trắng
    Vẽ Score

MAIN:
    Khởi tạo random
    Ẩn cursor
    setup
    draw

    Trong khi GameOver == false:
        input
        update
        draw
        Sleep(100)

    Hiển thị GAME OVER
    Chờ phím
    Kết thúc
```

---

# 32. Kết luận

Chương trình là một game Snake console được xây dựng theo mô hình vòng lặp:

```text
INPUT → UPDATE → DRAW → DELAY
```

Trong đó:

- `vector<Point>` quản lý thân rắn.
- `Point` quản lý tọa độ.
- `Direction` quản lý hướng.
- `food` quản lý thức ăn.
- `score` quản lý điểm.
- `gameOver` quản lý trạng thái kết thúc.
- `input()` xử lý điều khiển.
- `update()` xử lý logic trò chơi.
- `draw()` hiển thị trạng thái.
- `spawnFood()` tạo thức ăn.
- `setup()` khởi tạo trạng thái ban đầu.
- `main()` điều phối toàn bộ chương trình.

Thiết kế này thể hiện được các kiến thức C++ quan trọng gồm **struct, enum, vector, hàm, vòng lặp, điều kiện, con trỏ/handle của Windows API ở mức sử dụng thư viện, xử lý bàn phím, số ngẫu nhiên và tổ chức chương trình thành các chức năng riêng biệt**.

