#include<stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <dirent.h> 
#include <sys/stat.h> 
#include <utime.h> 
#include <time.h> 
#include <pwd.h> 
#include <grp.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <errno.h>
#include <string.h>

int print_type(struct stat st) {
	if (S_ISREG(st.st_mode))
	{
		printf("-");
	}
	else if (S_ISDIR(st.st_mode))
	{
		printf("d");
	}
	else if (S_ISCHR(st.st_mode))
	{
		printf("c");
	}
	else if (S_ISBLK(st.st_mode))
	{
		printf("b");
	}
	else if (S_ISFIFO(st.st_mode))
	{
		printf("p");
	}
	else if (S_ISLNK(st.st_mode))
	{
		printf("l");
	}
	else if (S_ISSOCK(st.st_mode))
	{
		printf("s");
	}
	else
	{
		printf("error\n");
		exit(-1);
	}
}

int print_perm(struct stat st) {
	if (st.st_mode & S_IRUSR)
	{
		printf("r");
	}
	if (st.st_mode & S_IWUSR)
	{
		printf("w");
	}
	if (st.st_mode & S_IXUSR)
	{
		printf("x");
	}
	
	if (st.st_mode & S_IRGRP)
	{
		printf("r");
	}
	if (st.st_mode & S_IWGRP)
	{
		printf("w");
	}
	if (st.st_mode & S_IXGRP)
	{
		printf("x");
	}

	if (st.st_mode & S_IROTH)
	{
		printf("r");
	}
	if (st.st_mode & S_IWOTH)
	{
		printf("w");
	}
	if (st.st_mode & S_IXOTH)
	{
		printf("x");
	}
	printf(" ");
}

int process_ls_l(char* path)
{
	DIR* dirp=NULL;
	struct dirent *direntp=NULL;
	char filename[100] = { 0 };
	struct stat st;
	int ret;
	dirp = opendir(path);
	if (dirp == NULL)
	{
		perror("opendir");
		exit(-1);
	}
	while ((direntp = readdir(dirp)) != NULL)
	{
		if (strncmp(direntp->d_name, ".", 1) == 0) continue;
		strcpy(filename, path);
		if (filename[strlen(filename) - 1] != '/')
		{
			strcat(filename, "/");
		}
		strcat(filename, direntp->d_name);
		// printf("filename:%s\n",filename);
		ret = stat(filename, &st);
		if (ret < 0)
		{
			perror("stat");
			exit(-1);
		}
	
		print_type(st);
		print_perm(st);
		printf("%ld", st.st_nlink);
		printf(" ");
		struct passwd* pw;
		pw = getpwuid(st.st_uid);
		printf("%s", pw->pw_name);
		printf(" ");
		struct group* gr;
		gr = getgrgid(st.st_gid);
		printf("%s", gr->gr_name);
		printf(" ");
		printf("%5ld", st.st_size);
		printf(" ");
		char *p_time;
		p_time = ctime(&st.st_mtime);
		for (int i = 0; p_time[i] != 0 && p_time[i] != '\n'; i++) {
			putchar(p_time[i]);
			printf(" ");
		}
		printf("%s\n", direntp->d_name);
	}
	closedir(dirp);
	return 0;
}
int main(int argc, char* argv[])
{
   if (argc == 3) // ./myls -l path
	{
	    process_ls_l(argv[2]);
	}
	else
	{
		printf("error\n");
		exit(-1);
	}
	return 0;
}
