#include <ncurses.h>
#include <unistd.h> // Para usar usleep

#define DELAY 60000   // Velocidade da bola
#define PADDLE_WIDTH 12 // Largura da plataforma (maior)

int main() {
    int max_x, max_y;              // Tamanho da tela
    int ball_x, ball_y;            // Posição inicial da bola
    int ball_dir_x = 1, ball_dir_y = 1; // Direção da bola
    int paddle_x, paddle_y;        // Posição da plataforma
    int score = 0;                 // Pontuação

    // Inicializa o ncurses
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE); // Permite usar teclas como as setas

    // Obtém o tamanho inicial da tela
    getmaxyx(stdscr, max_y, max_x);

    // Define a posição inicial da bola (centro da tela)
    ball_y = max_y / 2;
    ball_x = max_x / 2;

    // Define a posição inicial da plataforma
    paddle_y = max_y - 2;
    paddle_x = (max_x - PADDLE_WIDTH) / 2;

    while (1) {
        clear();

        // Atualiza o tamanho da tela em tempo real
        getmaxyx(stdscr, max_y, max_x);

        // Reposiciona a plataforma se necessário
        if (paddle_x + PADDLE_WIDTH > max_x) {
            paddle_x = max_x - PADDLE_WIDTH;
        }

        // Exibe a pontuação
        mvprintw(0, 0, "Pontuação: %d", score);

        // Desenha a bola
        mvprintw(ball_y, ball_x, "O");

        // Desenha a plataforma
        for (int i = 0; i < PADDLE_WIDTH; i++) {
            mvprintw(paddle_y, paddle_x + i, "=");
        }

        // Movimenta a bola
        ball_x += ball_dir_x;
        ball_y += ball_dir_y;

        // Verifica colisões com as bordas
        if (ball_x <= 0 || ball_x >= max_x - 1) {
            ball_dir_x *= -1; // Inverte direção horizontal
        }
        if (ball_y <= 0) {
            ball_dir_y *= -1; // Inverte direção vertical
        }

        // Verifica colisão com a plataforma
        if (ball_y == paddle_y - 1 && ball_x >= paddle_x && ball_x < paddle_x + PADDLE_WIDTH) {
            ball_dir_y *= -1; // Rebote vertical
            score++;          // Aumenta a pontuação
        }

        // Verifica se a bola caiu
        if (ball_y >= max_y) {
            mvprintw(max_y / 2, (max_x / 2) - 5, "GAME OVER!");
            mvprintw((max_y / 2) + 1, (max_x / 2) - 8, "Pontuação final: %d", score);
            refresh();
            usleep(2000000);
            break;
        }

        // Controle da plataforma
        int ch = getch();
        if (ch == KEY_LEFT && paddle_x > 0) {
            paddle_x--; // Move para a esquerda
        } else if (ch == KEY_RIGHT && paddle_x < max_x - PADDLE_WIDTH) {
            paddle_x++; // Move para a direita
        }

        refresh();
        usleep(DELAY); // Controla a velocidade da bola
    }

    // Finaliza o ncurses
    endwin();
    return 0;
}
