#include <stdio.h>

int main() {

  printf("\033[2J"); /*clear screen*/
  printf("\033[2;1H"); /*put in seperator between User input and chat text*/
  printf("-----------------------------\n"); /*barrier*/
  printf("\033[1;1H"); /*put at upper left*/
  printf(">some user text not done yet... "); /*user is in the middle of typing*/

  printf("\033[3;1H"); /*move to 2nd line on screen*/
  printf("\033[1L"); /*insert line here*/
  printf("comment one\n"); /*some text arrives*/
  printf("\033[1;1H"); /*go back to first row*/
  printf("\033[4~"); /*supposed to be "END" key but doesnt work (hitting the end key doesnt seem to work either, so I dont know what to put here)*/
  printf("more text... ");

  printf("\033[3;1H"); /*move to 2nd line on screen*/
  printf("\033[1L"); /*insert line here*/
  printf("comment two\n"); /*some more text arrives*/
  printf("\033[1;1H"); /*go back to first row*/
  printf("\033[4~"); /*supposed to be "END" key but doesnt work (hitting the end key doesnt seem to work either, so I dont know what to put here)*/

  char ui[255];

  fgets(ui, 255, stdin);
  printf("%s\n", ui);

  return 0;

}
