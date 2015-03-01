#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#include <libelf.h>

struct list_head {
    struct list_head *next;
};

#define container_of(ptr, type, member) \
    ((type *)( (char *)(ptr) - offsetof(type,member) ))

struct obj_t {
    Elf *elf;
    struct list_head list;
};

struct env_t {
    int elf_fd;
    Elf *arf;
    struct obj_t obj;
};

struct env_t *init(const char *file)
{
    struct env_t *env;
    Elf_Cmd cmd;
    Elf *elf;
    struct list_head *p;

    if(elf_version(EV_CURRENT) == EV_NONE)
    {
        /* library out of date */
        fprintf(stderr, "Library out of date.\n");
        exit(2);
    }
    env = malloc(sizeof(struct env_t));
    env->elf_fd = open(file, O_RDONLY);
    if(env->elf_fd == -1) {
        perror("open");
        exit(1);
    }
    cmd = ELF_C_READ;
    env->arf = elf_begin(env->elf_fd, cmd, NULL);
    p = &env->obj.list;
    p->next = NULL;
    while((elf=elf_begin(env->elf_fd, cmd, env->arf))!=0) {
        container_of(p, struct obj_t, list)->elf = elf;
        cmd = elf_next(elf);
        if(cmd==ELF_C_READ) {
            p->next = &((struct obj_t*)malloc(sizeof(struct obj_t)))->list;
            p = p->next;
            p->next = NULL;
        }
    }
    return env;
}

void fini(struct env_t **env)
{
    struct env_t *envp = *env;
    struct list_head *p;
    for(p=&envp->obj.list; p; p=p->next) {
        elf_end(container_of(p, struct obj_t, list)->elf);
    }
    if(envp->arf) {
        elf_end(envp->arf);
    }
    close(envp->elf_fd);
    free(envp);
    *env = NULL;
}

void process(struct env_t *env)
{
    struct list_head *p;
    const char * const kinds[] = {"Unknown", "Archive", "Stupid old COFF", "ELF file"};
    unsigned int cnt=0;

    printf("File type: %s\n", kinds[elf_kind(env->arf)]);
    for(p=&env->obj.list; p; p=p->next) {
        size_t s_ident;
        const char *ident;
        Elf *elf = container_of(p, struct obj_t, list)->elf;
        printf("-----Part %d-----\n", ++cnt);
        printf("Type: %s\n", kinds[elf_kind(elf)]);
        ident = elf_getident(elf, &s_ident);
        printf("Class: %d\n", ident[EI_CLASS]==ELFCLASS32?32:64);
    }
}

int main(int argc, char *argv[])
{
    struct env_t *env;
    if(argc<2) {
        exit(2);
    }
    env = init(argv[1]);
    process(env);
    fini(&env);
    return 0;
}

