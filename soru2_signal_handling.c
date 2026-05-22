#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
    Parent process'in child process'e sinyal gönderebilmesi için
    child process'in PID değeri global olarak tutulur.
*/
pid_t child_pid;

/*
    Alarm sayısını tutar.
    Parent process her SIGALRM aldığında bu değer artar.
*/
int alarm_count = 0;

/*
    Child process SIGINT aldığında çalışır.
    Normalde SIGINT programı sonlandırabilir.
    Ancak burada child process sadece mesaj yazdırır ve çalışmaya devam eder.
*/
void handle_sigint(int sig)
{
    printf("Çocuk: SIGINT alındı ancak devam ediliyor...\n");
}

/*
    Child process SIGCONT aldığında çalışır.
    SIGCONT, SIGSTOP ile durdurulmuş süreci devam ettirir.
*/
void handle_sigcont(int sig)
{
    printf("Çocuk: İşlem yeniden başlatıldı\n");
}

/*
    Parent process SIGALRM aldığında çalışır.
    Her alarmda child process önce durdurulur, sonra devam ettirilir.
*/
void handle_sigalrm(int sig)
{
    alarm_count++;

    printf("Ebeveyn: Çocuk durduruluyor...\n");
    kill(child_pid, SIGSTOP);

    sleep(2);

    printf("Ebeveyn: Çocuk devam ediyor...\n");
    kill(child_pid, SIGCONT);

    /*
        İlk iki alarmdan sonra tekrar alarm kuruyoruz.
        Böylece child process en az iki kez durdurulup devam ettirilmiş olur.
    */
    if (alarm_count < 2)
    {
        alarm(3);
    }
}

int main()
{
    child_pid = fork();

    if (child_pid < 0)
    {
        printf("Hata: fork işlemi başarısız oldu.\n");
        return 1;
    }

    /*
        CHILD PROCESS
    */
    if (child_pid == 0)
    {
        int counter = 0;

        signal(SIGINT, handle_sigint);
        signal(SIGCONT, handle_sigcont);

        while (1)
        {
            printf("Çocuk sayacı: %d\n", counter);
            counter++;
            sleep(1);
        }
    }

    /*
        PARENT PROCESS
    */
    else
    {
        signal(SIGALRM, handle_sigalrm);

        /*
            İlk alarm 3 saniye sonra çalışır.
        */
        alarm(3);

        /*
            Parent process, iki alarm gerçekleşene kadar bekler.
            pause(), herhangi bir sinyal gelene kadar süreci bekletir.
        */
        while (alarm_count < 2)
        {
            pause();
        }

        /*
            İki durdur/devam ettir işleminden sonra SIGINT gönderilir.
        */
        printf("Ebeveyn: SIGINT gönderiliyor...\n");
        kill(child_pid, SIGINT);

        sleep(1);

        /*
            SIGINT child tarafından yakalandığı için child ölmez.
            Bu yüzden sonlandırmak için SIGTERM gönderiyoruz.
        */
        kill(child_pid, SIGTERM);

        wait(NULL);

        printf("Ebeveyn: Çocuk sonlandırıldı.\n");
    }

    return 0;
}
