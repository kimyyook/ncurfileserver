#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/statfs.h>
#include<dirent.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>

#define MSIZE 16777216



int sorting(const struct dirent **a, const struct dirent **b)
{
	struct stat stt1;
	struct stat stt2;
	lstat((*a)->d_name, &stt1);
	lstat((*b)->d_name, &stt2);
	if((stt1.st_mode > stt2.st_mode)) 
		return 1;

	else return 0;
}

void init_scr()
{
	initscr();
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	curs_set(0);
	noecho();
	keypad(stdscr, TRUE);
}


void make_border()
{
	move(1, 0);		hline('=',80);
	move(22, 0);	hline('=',80);
	move(2, 39);	vline('|',20);
	move(2, 40);	vline('|',20);
	
}

void make_topbar(WINDOW *topbar, char* path)
{
	wbkgd(topbar, COLOR_PAIR(1));
	wprintw(topbar, "%80s","");
	wattron(topbar, COLOR_PAIR(1));
	mvwprintw(topbar, 0,0," Path : %s", path);
	wattron(topbar, COLOR_PAIR(1));
}

int delpopup(WINDOW *delpop, char * path, char * strname)
{
	char Answer[256] = {"\0"};	
	char correct[] = "y";
	char correct2[] = "Y";
	
	echo();	
	
	delpop = newwin(6, 50, 8, 15);
	wbkgd(delpop, COLOR_PAIR(1));
	
	wprintw(delpop, "%40s\n"," ");
	wprintw(delpop, "%2s Path : %s/%s %10s\n"," ",path, strname," ");
	wprintw(delpop, "%40s\n"," ");
	wprintw(delpop, "%2s DELETE Is it Okay? (y/n) :"," ");
	wgetstr(delpop, Answer);
	wprintw(delpop, "%40s\n"," ");
		
	if (strcmp(Answer,correct) == 0 || strcmp(Answer,correct2) == 0)
		return 1;
	else
		return 0;
	
	wrefresh(delpop);
	
	
}

void make_popup(WINDOW *popup, char* path, char* strname, char* strnametwo)
{
	char inputname[256] = {'\0'};	
	
	echo();	
	
	popup = newwin(6, 50, 8, 15);
	wbkgd(popup, COLOR_PAIR(1));
	
	wprintw(popup, "%40s\n"," ");
	wprintw(popup, "%2s Current Name : %s/%s %10s\n"," ",path,strname," ");
	wprintw(popup, "%40s\n"," ");
	wprintw(popup, "%2s Change Name : "," ");
	wgetstr(popup,inputname);
	wprintw(popup, "%40s\n"," ");
	
	strnametwo = inputname;
	
	
	
	
	rename(strname,strnametwo);
	
	wrefresh(popup);
}
char edit(char * temp){	
	WINDOW *editwin;
	
	char edittemp[512]={"\0"};
	echo();
	editwin = newwin(10,50,6,15);
	wbkgd(editwin, COLOR_PAIR(1));
	wprintw(editwin, "%40s\n"," ");
	wprintw(editwin, "%2s inputs some sentences\n");
	wgetstr(editwin, edittemp);
	
	temp = edittemp;
	

}
char editfile(char *path_name){
	char *temp = {"\0"};
	edit(temp);
}
void make_newfile(WINDOW *newfile, char* newfiles)
{
	
	int fd=0;
	char permit_str[10]={"\0"};
	char inputname[100] = {"\0"};
	char *temp = "file created";
	
	
	
	echo();
	newfile = newwin(6,50,8,15);
	wbkgd(newfile, COLOR_PAIR(1));
	
	wprintw(newfile, "%40s\n"," ");
	wprintw(newfile, "%2s file create... %10s\n"," "," ");
	wprintw(newfile, "%2s file name : "," ");
	wgetstr(newfile, inputname);
	wprintw(newfile, "%40s\n"," ");
	
	newfiles = inputname;
	
	edit(temp);
	
	if(0<(fd=creat(newfiles,0644)))
	{
		write(fd,temp,strlen(temp));
		close(fd);	
	}
	else
		wprintw(newfile, "%2s error!"," ");	
	
}

void make_dirfile(WINDOW *newdir, char * dirname,int count_num){
	
	
	char inputname[100] = {"\0"};	
	
	echo();
	newdir = newwin(6,50,8,15);
	wbkgd(newdir, COLOR_PAIR(1));
	
	wprintw(newdir, "%40s\n"," ");
	wprintw(newdir, "%40s\n"," ");
	wprintw(newdir, "%2s New DIrectory : "," ");
	wgetstr(newdir, inputname);
	wprintw(newdir, "%40s\n"," ");
	
	dirname = inputname;	
	mkdir(dirname, 0755);	
}



void make_bottombar(WINDOW *bottombar, int Dsize, int count, int Dcount, unsigned long freesize, unsigned long totalsize)
{
	count -= 2;  
	Dcount -= 2; 
	wbkgd(bottombar, COLOR_PAIR(1));
	wprintw(bottombar, "%80s","");
	wattron(bottombar, COLOR_PAIR(1)); 
	mvwprintw(bottombar,0,0," %8d File %8d Dir %14d Byte %14lu Byte free(%.1lf%)", 
			count-Dcount, Dcount, Dsize, freesize, (double)freesize/totalsize*100); 
}

void make_helpbar(WINDOW *helpbar){
	wbkgd(helpbar, COLOR_PAIR(2));
	wprintw(helpbar, "%80s","");
	wattron(helpbar, COLOR_PAIR(2)); 
	move(24, 0);	hline('=',80);
	move(27, 0);	hline('=',80);
	
	mvwprintw(helpbar,1,0,"%2s Press function key"," ");
	mvwprintw(helpbar,2,0,"%2s F(5): rename%6sF(6): new file%6sF(7): new directory%6sf(8): delete", " "," "," "," "); 
	
	
}

int authority(int file_mode, char *auth)
{
	if (S_ISLNK(file_mode))		sprintf(auth,"%c",'l');
	else if	(S_ISDIR(file_mode))	sprintf(auth,"%c",'d');
	else if(S_ISCHR(file_mode))	sprintf(auth,"%c",'c');
	else if(S_ISBLK(file_mode))	sprintf(auth,"%c",'b');	
	else if(S_ISFIFO(file_mode))	sprintf(auth,"%c",'p');
	else if(S_ISSOCK(file_mode))	sprintf(auth,"%c",'s');
	else				sprintf(auth,"%c",'-');

	if (file_mode & S_IRUSR)	sprintf(auth,"%s%c",auth,'r');
	else				sprintf(auth,"%s%c",auth,'-');
	if (file_mode & S_IWUSR)	sprintf(auth,"%s%c",auth,'w');
	else				sprintf(auth,"%s%c",auth,'-');
	if (file_mode & S_IXUSR)	sprintf(auth,"%s%c",auth,'x');
	else				sprintf(auth,"%s%c",auth,'-');

	if (file_mode & S_IRGRP)	sprintf(auth,"%s%c",auth,'r');
	else				sprintf(auth,"%s%c",auth,'-');
	if (file_mode & S_IWGRP)	sprintf(auth,"%s%c",auth,'w');
	else				sprintf(auth,"%s%c",auth,'-');
	if (file_mode & S_IXGRP)	sprintf(auth,"%s%c",auth,'x');
	else				sprintf(auth,"%s%c",auth,'-');

	if (file_mode & S_IROTH)	sprintf(auth,"%s%c",auth,'r');
	else				sprintf(auth,"%s%c",auth,'-');
	if (file_mode & S_IWOTH)	sprintf(auth,"%s%c",auth,'w');
	else				sprintf(auth,"%s%c",auth,'-');
	if (file_mode & S_IXOTH)	sprintf(auth,"%s%c",auth,'x');
	else				sprintf(auth,"%s%c",auth,'-');
}


void print_list(WINDOW *leftside, WINDOW *rightside, int count, int highlight, char list[count][39], int page, int slide)
{
	int i = 40*(page-1)+1;
	int j = 0;
	WINDOW *S;

        for(i; i<40*page+1 && i<count ; i++)                                                                                  
       	{       	
		if(((i-1)/20)%2==0)	S = leftside;
		else	S = rightside; 
		
		if(list[i][0]=='.' && list[i][1]=='.' && list[i][2]==' ') wattron(S, COLOR_PAIR(5));
		else if(list[i][22]=='d') wattron(S, COLOR_PAIR(3));
		else if(list[i][22]=='-' && list[i][25]=='x') wattron(S, COLOR_PAIR(4));
		else wattron(S, COLOR_PAIR(2));
		if(highlight == (i-1)%40+1)
	        {
			wattron(S, A_REVERSE);   
			mvwprintw(S,(i-1)%20,0,"%s",list[i]);                                                                
			wattroff(S, A_REVERSE);
	              }                                                                                                       
		else
		{
                        mvwprintw(S, (i-1)%20, 0, "%s", list[i]);
	        }
		wrefresh(leftside); 
		wrefresh(rightside); 
	}
	
	if(i>=count-1)
	{
		for(i; i<((count-1)/40+1)*40+1; i++)
		{
			if(((i-1)/20)%2==0)	S = leftside;
			else	S = rightside;
			mvwprintw(S,(i-1)%20,0,"%39s"," ");
			wrefresh(leftside); 
			wrefresh(rightside);
		}
	}
}



int Remove(char * highlight){	
	char arr[500];
	char path_name[1024];
	char path_name1[255];
	char buffer[255];
	
	int dir_num=0,i=0;
	struct stat file_info;					
	mode_t file_mode;							
	struct dirent **new_direntp;
									
	DIR *dirp;								
	chdir(highlight);				
	getcwd(path_name,255);
												
	dir_num=scandir(path_name,&new_direntp,NULL,alphasort);
													
	for(i=2;i<dir_num;i++)												
	{
		stat(new_direntp[i]->d_name, &file_info);
		file_mode = file_info.st_mode;
																						
		if(S_ISDIR(file_mode)){
			Remove(new_direntp[i]->d_name);	
		}
					
		else
		{
			unlink(new_direntp[i]->d_name);
		}
	}
	chdir("..");
	rmdir(highlight);
}
			

int main()
{
	WINDOW *topbar, *leftside, *rightside, *bottombar, *helpbar, *popup, *newfile, *newdir, *delpop;
	struct dirent **direntp;
	struct dirent *temp;
	struct stat stti, sttj;
	struct statfs sfs;
	char * Thefilename = "\0";
	char * filename_old = "\0";
	char * filename_new = "\0";
	char * newfilename = "\0";
	char * newdirname = '\0';
	
	char path_name[255];
	char strPath[255];
	char delete_path[100];
	
	int quest=0;
	int morecount=0;
	int count_num=0;
	int key = 0, highlight = 1;
	int page = 1, slide = 1;
	int count = 0, Dcount = 0, Dsize = 0;
	unsigned long totalsize = 0, freesize = 0;
	int i=0, j=0;

	init_scr();
	bkgd(COLOR_PAIR(2));
        
      
	topbar = subwin(stdscr,1, 80, 0, 0);
	bottombar = subwin(stdscr, 1, 80, 23, 0);
	helpbar=subwin(stdscr, 4, 80, 24, 0);
	leftside = subwin(stdscr, 20, 39, 2, 0);
	rightside = subwin(stdscr, 20, 39, 2, 41);
	

	getcwd(path_name,255);

	count = scandir(path_name, &direntp, 0, sorting); 
	char permission[count][10];
	char list[count][39];	
	char cutname[count][23];

	
 	statfs("/", &sfs);
	totalsize = sfs.f_bsize * sfs.f_blocks;
	freesize = sfs.f_bsize * sfs.f_bavail;

  
	for(i = 0; i<count; i++)
	{
		lstat(direntp[i]->d_name,&stti);
		for(j = i; j<count; j++)
		{
			lstat(direntp[j]->d_name,&sttj);
			if((!S_ISDIR(stti.st_mode)) && (S_ISDIR(sttj.st_mode)))
			{	
				temp = direntp[i];
				direntp[i] = direntp[j];
				direntp[j] = temp;
			}
			else if((S_ISDIR(stti.st_mode)) && (S_ISDIR(sttj.st_mode)))
			{	
				if((strcmp(direntp[i]->d_name,direntp[j]->d_name))>0)
				{
					temp = direntp[i];
					direntp[i] = direntp[j];
					direntp[j] = temp;
				}
			}
			lstat(direntp[i]->d_name,&stti);
		}
	}
	
	for(i = 0; i<count; i++)
	{
		lstat(direntp[i]->d_name,&stti);
		for(j = i; j<count; j++)
		{
			lstat(direntp[j]->d_name,&sttj);
			if(((!S_ISLNK(stti.st_mode)) &&(!S_ISDIR(stti.st_mode)) && (!S_ISCHR(stti.st_mode)) && 
			(!S_ISBLK(stti.st_mode)) && (!S_ISFIFO(stti.st_mode)) && (!S_ISSOCK(stti.st_mode))) && 
			((!S_ISLNK(sttj.st_mode)) && (!S_ISDIR(sttj.st_mode)) && (!S_ISCHR(sttj.st_mode)) && 
			(!S_ISBLK(sttj.st_mode)) && (!S_ISFIFO(sttj.st_mode)) && (!S_ISSOCK(sttj.st_mode))))
			{	
				temp = direntp[i];
				direntp[i] = direntp[j];
				direntp[j] = temp;
			}
			lstat(direntp[i]->d_name,&stti);
			authority(stti.st_mode, permission[i]);
		}
	}

	
	for(i = 0; i<count; i++)
	{
		lstat(direntp[i]->d_name,&stti);
		for(j = i; j<count; j++)
		{
			lstat(direntp[j]->d_name,&sttj);
			if((permission[i][0]==permission[j][0]) && (permission[i][3] == permission[j][3]))
			{	
				if((strcmp(direntp[i]->d_name,direntp[j]->d_name))>0)
				{
					temp = direntp[i];
					direntp[i] = direntp[j];
					direntp[j] = temp;
				}
			}
			lstat(direntp[i]->d_name,&stti);
		}
	}

	
	for(i = 0; i<count; i++)
	{
		lstat(direntp[i]->d_name,&stti);
		authority(stti.st_mode, permission[i]);
		strncpy(cutname[i],direntp[i]->d_name,22);
		cutname[i][22]='\0'; 
		Dsize += (int)stti.st_size;
		sprintf(list[i], "%-22s%-10s%7d", cutname[i], permission[i], (int)stti.st_size);
		if(S_ISDIR(stti.st_mode))
			Dcount++;
	}

	wbkgd(leftside,	 COLOR_PAIR(2));
	wbkgd(rightside, COLOR_PAIR(2));

	
	make_border();
	make_topbar(topbar, path_name);
	make_bottombar(bottombar, Dsize, count, Dcount, freesize, totalsize);
	make_helpbar(helpbar);
	refresh();


	strcpy(list[1],"..                                    "); 
	list[1][38]=' '; 

	
	print_list(leftside, rightside, count, highlight, list, page, slide);
	while(1)
	{
		wattron(stdscr, COLOR_PAIR(1));
		mvwprintw(stdscr, 0,70, "Page : %2d", page); 
	        key = wgetch(stdscr);
		switch(key)
		{ 	case KEY_UP:                                                                                    
                                if(highlight == 1 && slide>1)   
				{	
					if(slide%2==1)
					{
						page--;
						highlight = 40;
						print_list(leftside, rightside, count, highlight, list, page, slide);
					}
					slide--;
				}
                                else if(highlight == 1 && slide == 1)
					break;
 				else 
				{
					if(highlight==21) slide--;
					--highlight;
				}
                                break;                
                        case KEY_DOWN:
                                if(highlight == 40)
				{
                                        highlight = 1;
					slide++;
					page++;
				}
				else if(count == highlight+((page-1)*40)+1) 
					break;
				
                                else 
				{
					if(highlight == 20)
						slide++;
					++highlight;
				}
                                break;
			case KEY_RIGHT: 
					
				if(1+((slide)*20) > count)
					break;
				if(highlight>20)
				{
					highlight = 1;
					slide++;
					page++;
				}
				else
				{
					if(highlight+21+((slide-1)*20) > count){
						slide++;
						highlight = 21;

					}
					else{
						highlight += 20;
						slide++;
					}
				}
				
				break;
			case KEY_LEFT: 
					
				if(slide==1)
					break;
				else if(highlight<20)
				{
					highlight = 21;
					slide--;
					page--;
				}
				else
				{
					highlight -= 20;
					slide--;
				}
				
				break;
			case 10: 
				if(permission[highlight+((page-1)*40)][0]=='d') 
				{	
					sprintf(path_name, "%s/%s", path_name, direntp[highlight+((page-1)*40)]->d_name);
					chdir(path_name);
					main();
					return 0;
				}
				else if(permission[highlight+((page-1)*40)][0]=='-'&& permission[highlight+((page-1)*40)][3]=='x'&& permission[highlight+((page-1)*40)][9]=='x') //파일 실행 
				{
					endwin();
					execl(direntp[highlight+((page-1)*40)]->d_name, direntp[highlight+((page-1)*40)]->d_name,NULL);
				}
				else{
					editfile(direntp[highlight+((page-1)*40)]->d_name);
					main();
					break;
				
				}
			
			case KEY_F(5):
				filename_old = (direntp[highlight+((page-1)*40)]->d_name);
			    filename_new = "\0";
				make_popup(popup, path_name,filename_old, filename_new);
				endwin();
				main();
				break;
				
					 
			case KEY_F(6):
				newfilename = "\0";
				make_newfile(newfile,newfilename);
				endwin();
				main();
				break;
			
			
			
			case KEY_F(7):
				newdirname = "\0";
				make_dirfile(newdir,newdirname,count_num);
				endwin();
				main();
				break;
			
			case KEY_F(8):
				Thefilename = (direntp[highlight+((page-1)*40)]->d_name);
				
				if(strcmp(Thefilename,"..")==0)
				{
					endwin();
					break;
				}
				else{
					quest=delpopup(delpop, path_name, Thefilename);
				
					endwin();
					if(quest==1){
						if(permission[highlight+((page-1)*40)][0]=='d')
						{
							Remove(direntp[highlight+((page-1)*40)]->d_name);
							main();
						}
						else
						{
							unlink(direntp[highlight+((page-1)*40)]->d_name);
							main();
						}
						break;			
					}
					else{
						endwin();
						main();
						break;
					}
				}
				
			case 27: 
				delwin(topbar);
				delwin(bottombar);
				delwin(helpbar);
				delwin(leftside);
				delwin(rightside);
				delwin(stdscr);
				endwin();
				return 0;

                        default:
                                refresh();
                                break;
		}
		print_list(leftside, rightside, count, highlight, list, page, slide);
	}
}
