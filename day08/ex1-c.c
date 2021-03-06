/* ex1-c: Advent2020 game, day 8/game 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct instr {
    struct instr *next;
    int instr;
    int val;
    int visited;
};

struct program {
    unsigned length;
    unsigned total;
    struct instr *prg;
};

#define BLOCKSIZE	1024    /* number of elements for realloc() */
#define	NOP		0
#define	ACC		1
#define	JMP		2

void print_instr(prg, step, cur)
    struct program *prg;
    int step;
    int cur;
{
    struct instr *instr=prg->prg+step;
    char *op="unknown";

    switch (instr->instr) {
        case NOP:
            op="nop";
            break;
        case JMP:
            op="jmp";
            break;
        case ACC:
            op="acc";
            break;
    }
    fprintf(stderr, "%03d: [%s][%4d] -> %4d\n", step, op, instr->val, cur);
}

void print_prg(prg)
    struct program *prg;
{
    unsigned i, psize=prg->length;
    struct instr *pinstr=prg->prg;

    fprintf(stderr, "PROGRAM: address=%p pinstr=%p size=%d\n", prg, pinstr, psize);
    for (i=0; i<psize; ++i)
        print_instr(prg, i, 0);
}

static struct program *add_line(prg, line)
    struct program *prg;
    char *line;
{
    char sinstr[80];
    int val;
    unsigned cur, total;
    struct instr *pinstr;

    if (!prg) {
        prg=malloc(sizeof (struct program));
        prg->prg=malloc(sizeof(struct instr)*BLOCKSIZE);

        //fprintf(stderr, "alloc buf: prg=%p prog=%p\n", prg, prg->prg);
        prg->length=0;
        prg->total=BLOCKSIZE;
    }
    cur=prg->length;
    total=prg->total;

    if (cur == total) {
        total+=BLOCKSIZE;
        prg->total=total;
        prg->prg=realloc(prg->prg, sizeof(struct instr)*total);
        //fprintf(stderr, "realloc buf: cur=%d total=%d prog=%p\n", cur, total, prg->prg);
    }
    pinstr=prg->prg+cur;

    sscanf(line, "%s %d", sinstr, &val);
    pinstr->visited=0;
    pinstr->val=val;
    if (!strcmp(sinstr, "jmp"))
        pinstr->instr=JMP;
    else if (!strcmp(sinstr, "acc"))
        pinstr->instr=ACC;
    else pinstr->instr=NOP;
    prg->length++;

    return prg;
}

int run(prg)
    struct program *prg;
{
    int cur=0, length=prg->length;
    struct instr *instr=prg->prg;
    int res=0;

    //fprintf(stderr, "RUN: address=%p prg=%p size=%d\n", prg, instr, length);
    while (cur>=0 && cur<length) {
        instr=prg->prg+cur;
        //print_instr(prg, cur, res);
        if (instr->visited) {
            break;
        }
        instr->visited=1;
        switch (instr->instr) {
            case NOP:
                cur++;
                break;
            case JMP:
                cur+=instr->val;
                break;
            case ACC:
                res+=instr->val;
                cur++;
                break;
        }
    }
    return res;
}

int main(ac, av)
    char **av;
{
    char line[512];
    struct program *prg=NULL;

    while (fgets(line, sizeof line, stdin)) {
        prg=add_line(prg, line);
    }
    //print_prg(prg);
    printf("%s : res=%d\n", *av, run(prg));
    exit(0);
}
