#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define MAX_LINE 1024
#define MAX_ITEMS 100
#define QUESTION_FOLDER "C:\\Users\\huang\\Desktop\\acm_systems\\题库"
#define QUESTION_INDEX  "C:\\Users\\huang\\Desktop\\acm_systems\\题库\\用户账号.txt"
#define ADMIN_KEY_HASH "teamwork"
#define MAX_CHANGES 3

typedef struct {
	char username[100];
	char password[100];
	int remaining_changes;
} UserInfo;

UserInfo g_currentUserInfo;

#define USER_FILE        "C:\\Users\\huang\\Desktop\\acm_systems\\用户账号.txt"
#define WINNER_FILE      "C:\\Users\\huang\\Desktop\\acm_systems\\历届获奖.txt"
#define QUESTION_FILE    "C:\\Users\\huang\\Desktop\\acm_systems\\题库\\题库索引.txt"
#define EVENT_FILE       "C:\\Users\\huang\\Desktop\\acm_systems\\赛事介绍.txt"
#define RULE_FILE        "C:\\Users\\huang\\Desktop\\acm_systems\\比赛规则.txt"
#define SCORE_FILE       "C:\\Users\\huang\\Desktop\\acm_systems\\评分细则.txt"
#define STRUCTURE_FILE   "C:\\Users\\huang\\Desktop\\acm_systems\\赛事构成.txt"

int  g_login = 0;
char g_curUser[100] = "";

void setColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

#define COLOR_DEFAULT 7
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_BLUE 9
#define COLOR_MAGENTA 13
#define COLOR_CYAN 11
#define COLOR_WHITE 15
#define COLOR_HIGHLIGHT 112

void printTitle() {
	setColor(COLOR_CYAN);
	printf("                                      \n");
	printf("      _    _____ _____ _____ _____   \n");
	printf("     / \\  /  __//  __//  __//  ___/  \n");
	printf("    / _ \\ | |   | |   | |   | |___   \n");
	printf("   / /_\\ \\| |   | |   | |   \\___  \\ \n");
	printf("  / _____ \\| |__ | |__ | |__ ___/ /  \n");
	printf(" /_/     \\_\\____/\\____/\\____/_____/  \n");
	printf("                                      \n");
	printf("      竞赛管理系统 v1.0              \n");
	printf("                                      \n");
	setColor(COLOR_DEFAULT);
}

void printBanner(const char* text) {
	setColor(COLOR_YELLOW);
	printf("╔");
	for(int i = 0; i < strlen(text) + 10; i++) printf("═");
	printf("╗\n");
	printf("║");
	for(int i = 0; i < 5; i++) printf(" ");
	printf("%s", text);
	for(int i = 0; i < 5; i++) printf(" ");
	printf("║\n");
	printf("╚");
	for(int i = 0; i < strlen(text) + 10; i++) printf("═");
	printf("╝\n");
	setColor(COLOR_DEFAULT);
}

void cls() { 
	system("cls"); 
}

void pause() { 
	setColor(COLOR_GREEN);
	printf("\n  [按任意键继续...]");
	setColor(COLOR_DEFAULT);
	getch(); 
}

void readPwd(char *dst, int showStar) {
	int idx = 0; char ch;
	while (1) {
		ch = getch();
		if (ch == 13) { dst[idx] = '\0'; printf("\n"); break; }
		if (ch == 8  && idx > 0) { --idx; printf("\b \b"); continue; }
		if (ch == 8) continue;
		dst[idx++] = ch;
		if (showStar) {
			setColor(COLOR_YELLOW);
			printf("*");
			setColor(COLOR_DEFAULT);
		}
	}
}

void showFile(const char* path) {
	FILE* fp = fopen(path, "r");
	if (!fp) { 
		setColor(COLOR_RED);
		printf("文件无法打开: %s\n", path); 
		setColor(COLOR_DEFAULT);
		return; 
	}
	char line[MAX_LINE];
	setColor(COLOR_BLUE);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	while (fgets(line, sizeof(line), fp)) {
		if (line[strlen(line)-1] == '\n') line[strlen(line)-1] = '\0';
		printf("  %s\n", line);
	}
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	setColor(COLOR_DEFAULT);
	fclose(fp);
}

int login() {
	static int attempt = 0, code_user, code_sys;
	srand((unsigned)time(NULL));
	retry:
	if (attempt == 5) { 
		setColor(COLOR_RED);
		printf("请先等待30秒后再试！\n"); 
		setColor(COLOR_DEFAULT);
		Sleep(30000); attempt = 0; 
	}
	code_sys = 1000 + rand() % 9000;
	cls();
	printTitle();
	setColor(COLOR_CYAN);
	printf("╔════════════════════════════════════════════════════════════╗\n");
	printf("║                    用户登录中心                            ║\n");
	printf("╚════════════════════════════════════════════════════════════╝\n");
	setColor(COLOR_DEFAULT);
	
	printf("\n");
	setColor(COLOR_YELLOW);
	printf("  验证码: %d\n", code_sys);
	setColor(COLOR_DEFAULT);
	
	printf("  请输入验证码: ");
	int scanf_result = scanf("%d", &code_user);
	
	if (scanf_result != 1) {
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
		setColor(COLOR_RED);
		printf("    输入无效！请输入数字验证码。\n");
		setColor(COLOR_DEFAULT);
		++attempt;
		pause();
		goto retry;
	}
	
	if (code_user != code_sys) { 
		setColor(COLOR_RED);
		printf("    验证码错误！\n"); 
		setColor(COLOR_DEFAULT);
		++attempt;
		pause(); 
		goto retry; 
	}
	
	char username[100], password[100];
	printf("  用户名: "); scanf("%s", username);
	printf("  密  码: "); readPwd(password, 1);
	
	FILE* fp = fopen(USER_FILE, "r");
	if (!fp) { 
		setColor(COLOR_RED);
		printf("   用户数据库未初始化！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return 0; 
	}
	
	char line[MAX_LINE], user[100], pass[100];
	int found = 0, changes = 0;  // 添加changes变量
	
	while (fgets(line, sizeof(line), fp)) {
		// 读取3个字段：用户名、密码、剩余次数
		if (sscanf(line, "%99s %99s %d", user, pass, &changes) >= 3) {
			if (strcmp(user, username) == 0 && strcmp(pass, password) == 0) {
				found = 1;
				break;
			}
		} else if (sscanf(line, "%99s %99s", user, pass) == 2) {
			if (strcmp(user, username) == 0 && strcmp(pass, password) == 0) {
				found = 1;
				changes = MAX_CHANGES;  // 旧用户默认为3次
				break;
			}
		}
	}
	fclose(fp);
	
	if (found) {
		strcpy(g_curUser, username);
		strcpy(g_currentUserInfo.username, username);
		strcpy(g_currentUserInfo.password, password);
		g_currentUserInfo.remaining_changes = (changes > 0) ? changes : MAX_CHANGES;
		
		setColor(COLOR_GREEN);
		printf("\n   登录成功！欢迎回来，%s！\n", username);
		printf("   剩余修改次数: %d\n", g_currentUserInfo.remaining_changes);
		setColor(COLOR_DEFAULT);
		pause(); 
		return 1;
	} else {
		++attempt; 
		setColor(COLOR_RED);
		printf("   用户名或密码错误！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); 
		goto retry;
	}
}

int registerUser() {
	char username[100], pwd1[100], pwd2[100];
	cls();
	printTitle();
	setColor(COLOR_CYAN);
	printf("╔════════════════════════════════════════════════════════════╗\n");
	printf("║                    用户注册中心                            ║\n");
	printf("╚════════════════════════════════════════════════════════════╝\n");
	setColor(COLOR_DEFAULT);
	
	printf("\n  用户名: "); scanf("%s", username);
	printf("  输入密码: "); readPwd(pwd1, 0);
	printf("  再次输入: "); readPwd(pwd2, 1);
	
	if (strcmp(pwd1, pwd2) != 0) { 
		setColor(COLOR_RED);
		printf("\n   两次密码不一致！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return 0; 
	}
	
	FILE* fp = fopen(USER_FILE, "r");
	if (fp) {
		char line[MAX_LINE], user[100];
		while (fgets(line, sizeof(line), fp)) {
			if (sscanf(line, "%99s", user) == 1 && strcmp(user, username) == 0) {
				fclose(fp); 
				setColor(COLOR_RED);
				printf("\n   用户 %s 已存在，无法注册\n", username); 
				setColor(COLOR_DEFAULT);
				pause(); return 0;
			}
		}
		fclose(fp);
	}
	
	fp = fopen(USER_FILE, "a");
	if (!fp) { 
		setColor(COLOR_RED);
		printf("\n   无法写入用户文件！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return 0; 
	}
	fprintf(fp, "%s %s %d\n", username, pwd1, MAX_CHANGES); fclose(fp);
	strcpy(g_curUser, username);
	
	// 立即初始化当前用户信息
	strcpy(g_currentUserInfo.username, username);
	strcpy(g_currentUserInfo.password, pwd1);
	g_currentUserInfo.remaining_changes = MAX_CHANGES;
	
	setColor(COLOR_GREEN);
	printf("\n   注册成功！已自动登录。\n");
	printf("   剩余修改次数: %d\n", MAX_CHANGES);
	setColor(COLOR_DEFAULT);
	pause(); return 1;
}

void changePassword() {
	char username[100], newPwd1[100], newPwd2[100];
	cls();
	printBanner("修改密码");
	printf("\n");
	
	printf("  请输入用户名: "); scanf("%s", username);
	
	// 检查用户是否存在
	FILE* fp = fopen(USER_FILE, "r");
	if (!fp) { 
		setColor(COLOR_RED);
		printf("   用户数据库未初始化！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	char line[MAX_LINE], user[100], pass[100];
	int found = 0, lineCount = 0, targetLine = -1;
	char users[MAX_ITEMS][100], passwords[MAX_ITEMS][100];
	
	while (fgets(line, sizeof(line), fp)) {
		if (sscanf(line, "%99s %99s", user, pass) == 2) {
			strcpy(users[lineCount], user);
			strcpy(passwords[lineCount], pass);
			if (strcmp(user, username) == 0) { found = 1; targetLine = lineCount; }
			lineCount++;
		}
	}
	fclose(fp);
	
	if (!found) { 
		setColor(COLOR_RED);
		printf("   用户 %s 不存在\n", username); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	// 直接输入新密码
	printf("\n  请输入新密码: "); readPwd(newPwd1, 0);
	printf("  请再次输入新密码: "); readPwd(newPwd2, 1);
	if (strcmp(newPwd1, newPwd2) != 0) { 
		setColor(COLOR_RED);
		printf("\n   两次新密码输入不一致！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	strcpy(passwords[targetLine], newPwd1);
	fp = fopen(USER_FILE, "w");
	if (!fp) { 
		setColor(COLOR_RED);
		printf("   无法写入用户文件！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	for (int i = 0; i < lineCount; i++) fprintf(fp, "%s %s\n", users[i], passwords[i]);
	fclose(fp);
	setColor(COLOR_GREEN);
	printf("   密码修改成功！\n");
	setColor(COLOR_DEFAULT);
	pause();
}

int deleteUser() {
	char password[100];
	cls();
	printBanner("注销账户");
	printf("\n");
	
	printf("  警告：注销账户将永久删除您的所有数据！\n\n");
	printf("  请输入当前登录用户的密码: "); readPwd(password, 1);
	
	FILE* fp = fopen(USER_FILE, "r");
	if (!fp) { 
		setColor(COLOR_RED);
		printf("   用户数据库未初始化！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return 0; 
	}
	
	char line[MAX_LINE], user[100], pass[100];
	int lineCount = 0, targetLine = -1;
	char users[MAX_ITEMS][100], passwords[MAX_ITEMS][100];
	int passwordCorrect = 0;
	
	while (fgets(line, sizeof(line), fp)) {
		if (sscanf(line, "%99s %99s", user, pass) == 2) {
			strcpy(users[lineCount], user);
			strcpy(passwords[lineCount], pass);
			if (strcmp(user, g_curUser) == 0 && strcmp(pass, password) == 0) passwordCorrect = 1;
			lineCount++;
		}
	}
	fclose(fp);
	
	if (!passwordCorrect) { 
		setColor(COLOR_RED);
		printf("   密码验证失败！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return 0; 
	}
	
	// 二次确认
	printf("  确认注销账户 %s 吗？此操作不可恢复 (Y/N): ", g_curUser);
	char confirm; scanf(" %c", &confirm);
	if (confirm != 'Y' && confirm != 'y') { 
		setColor(COLOR_YELLOW);
		printf("  已取消注销操作。\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return 0; 
	}
	
	fp = fopen(USER_FILE, "w");
	if (!fp) { 
		setColor(COLOR_RED);
		printf("   无法写入用户文件！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return 0; 
	}
	for (int i = 0; i < lineCount; i++) {
		if (strcmp(users[i], g_curUser) != 0) {
			fprintf(fp, "%s %s\n", users[i], passwords[i]);
		}
	}
	fclose(fp);
	
	setColor(COLOR_GREEN);
	printf("   账户 %s 注销成功！\n", g_curUser);
	printf("   正在返回登录界面...\n");
	setColor(COLOR_DEFAULT);
	g_login = 0; pause(); return 1;
}

/* 权限管理*/
int loadUserInfo(const char* username, UserInfo* userInfo) {
	FILE* fp = fopen(USER_FILE, "r");
	if (!fp) return 0;
	
	char line[MAX_LINE], user[100], pass[100];
	int changes = 0;
	
	while (fgets(line, sizeof(line), fp)) {
		// 尝试读取3个字段）
		if (sscanf(line, "%99s %99s %d", user, pass, &changes) >= 3) {
			if (strcmp(user, username) == 0) {
				strcpy(userInfo->username, user);
				strcpy(userInfo->password, pass);
				userInfo->remaining_changes = (changes > 0) ? changes : MAX_CHANGES;
				fclose(fp); 
				return 1;
			}
		} 
		else if (sscanf(line, "%99s %99s", user, pass) == 2) {
			if (strcmp(user, username) == 0) {
				strcpy(userInfo->username, user);
				strcpy(userInfo->password, pass);
				userInfo->remaining_changes = MAX_CHANGES;  // 用户默认3次
				fclose(fp); 
				return 1;
			}
		}
	}
	fclose(fp); 
	return 0;
}

void saveUserInfo(const UserInfo* userInfo) {
	FILE* fp = fopen(USER_FILE, "r");
	UserInfo users[MAX_ITEMS]; int count = 0;
	
	if (fp) {
		char line[MAX_LINE], user[100], pass[100]; int changes;
		while (fgets(line, sizeof(line), fp)) {
			if (sscanf(line, "%99s %99s %d", user, pass, &changes) >= 2) {
				strcpy(users[count].username, user);
				strcpy(users[count].password, pass);
				users[count].remaining_changes = (changes > 0) ? changes : MAX_CHANGES;
				count++;
			}
		}
		fclose(fp);
	}
	
	for (int i = 0; i < count; i++)
		if (strcmp(users[i].username, userInfo->username) == 0) { users[i] = *userInfo; break; }
	
	fp = fopen(USER_FILE, "w");
	if (fp) {
		for (int i = 0; i < count; i++) fprintf(fp, "%s %s %d\n", users[i].username, users[i].password, users[i].remaining_changes);
		fclose(fp);
	}
}

int checkAndConsumeChange() {
	if (strcmp(g_currentUserInfo.username, "admin") == 0) return 1;
	if (g_currentUserInfo.remaining_changes <= 0) {
		setColor(COLOR_RED);
		printf("\n   您的修改次数已用完！\n");
		setColor(COLOR_YELLOW);
		printf("   请申请重置次数或使用密钥。\n");
		setColor(COLOR_DEFAULT);
		pause(); return 0;
	}
	g_currentUserInfo.remaining_changes--;
	saveUserInfo(&g_currentUserInfo);
	setColor(COLOR_GREEN);
	printf("\n   操作成功！剩余修改次数: %d\n", g_currentUserInfo.remaining_changes);
	setColor(COLOR_DEFAULT);
	return 1;
}

int adminLogin() {
	char inputKey[100], hashed[100];
	cls();
	printTitle();
	setColor(COLOR_MAGENTA);
	printf("╔════════════════════════════════════════════════════════════╗\n");
	printf("║                  管理员身份验证                            ║\n");
	printf("╚════════════════════════════════════════════════════════════╝\n");
	setColor(COLOR_DEFAULT);
	
	printf("\n  请输入管理员密钥: "); readPwd(inputKey, 1);
	sprintf(hashed, "%s", inputKey);
	if (strcmp(hashed, ADMIN_KEY_HASH) == 0) {
		strcpy(g_currentUserInfo.username, "admin");
		strcpy(g_currentUserInfo.password, "admin");
		g_currentUserInfo.remaining_changes = 999;
		setColor(COLOR_GREEN);
		printf("\n   管理员身份验证成功！\n");
		setColor(COLOR_DEFAULT);
		pause(); return 1;
	} else { 
		setColor(COLOR_RED);
		printf("\n   密钥错误！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return 0; 
	}
}

void adminMenu() {
	while (1) {
		cls();
		printTitle();
		setColor(COLOR_MAGENTA);
		printf("╔════════════════════════════════════════════════════════════╗\n");
		printf("║                    管理员菜单                              ║\n");
		printf("╠════════════════════════════════════════════════════════════╣\n");
		printf("║  【1】 查看待审批申请                                      ║\n");
		printf("║  【2】 生成重置密钥                                        ║\n");
		printf("║  【3】 查看所有用户                                        ║\n");
		printf("║  【4】 删除用户                                            ║\n");
		printf("║  【0】 退出管理员模式                                      ║\n");
		printf("╚════════════════════════════════════════════════════════════╝\n");
		setColor(COLOR_DEFAULT);
		
		printf("\n  请选择（0-4）: ");
		int sel; scanf("%d", &sel); while (getchar() != '\n');
		if (sel == 0) { loadUserInfo(g_curUser, &g_currentUserInfo); break; }
		if (sel == 1) approveApplication();
		if (sel == 2) generateResetKey();
		if (sel == 3) viewAllUsers();
		if (sel == 4) adminDeleteUser();
	}
}

void viewAllUsers() {
	cls();
	setColor(COLOR_CYAN);
	printf("╔════════════════════════════════════════════════════════════╗\n");
	printf("║                    所有用户列表                            ║\n");
	printf("╠════════════════════════════════════════════════════════════╣\n");
	setColor(COLOR_DEFAULT);
	
	FILE* fp = fopen(USER_FILE, "r");
	if (!fp) {
		setColor(COLOR_RED);
		printf("║  用户数据库无法访问！                                       ║\n");
		setColor(COLOR_DEFAULT);
		printf("╚════════════════════════════════════════════════════════════╝\n");
		pause(); return;
	}
	
	char line[MAX_LINE], username[100], password[100];
	int count = 0;
	
	while (fgets(line, sizeof(line), fp)) {
		if (sscanf(line, "%99s %99s", username, password) == 2) {
			printf("║  %-3d │ %-20s │ %s\n", ++count, username, 
				   strcmp(username, "admin") == 0 ? "管理员" : "普通用户");
		}
	}
	fclose(fp);
	
	printf("╚════════════════════════════════════════════════════════════╝\n");
	printf("  共 %d 个用户\n", count);
	pause();
}

void adminDeleteUser() {
	char targetUser[100];
	cls();
	printBanner("删除用户");
	printf("\n");
	
	// 先显示用户列表
	viewAllUsers();
	
	printf("  请输入要删除的用户名: ");
	scanf("%99s", targetUser);
	
	// 不能删除admin账户
	if (strcmp(targetUser, "admin") == 0) {
		setColor(COLOR_RED);
		printf("   不能删除管理员账户！\n");
		setColor(COLOR_DEFAULT);
		pause(); return;
	}
	
	// 二次确认
	printf("  确认删除用户 %s 吗？此操作不可恢复 (Y/N): ", targetUser);
	char confirm; scanf(" %c", &confirm);
	if (confirm != 'Y' && confirm != 'y') {
		setColor(COLOR_YELLOW);
		printf("  已取消删除。\n");
		setColor(COLOR_DEFAULT);
		pause(); return;
	}
	
	// 执行删除
	FILE* fp = fopen(USER_FILE, "r");
	if (!fp) {
		setColor(COLOR_RED);
		printf("   用户数据库无法访问！\n");
		setColor(COLOR_DEFAULT);
		pause(); return;
	}
	
	char line[MAX_LINE], user[100], pass[100];
	int userCount = 0;
	char users[MAX_ITEMS][100], passwords[MAX_ITEMS][100];
	
	while (fgets(line, sizeof(line), fp)) {
		if (sscanf(line, "%99s %99s", user, pass) == 2) {
			strcpy(users[userCount], user);
			strcpy(passwords[userCount], pass);
			userCount++;
		}
	}
	fclose(fp);
	
	// 查找并删除目标用户
	int found = 0;
	for (int i = 0; i < userCount; i++) {
		if (strcmp(users[i], targetUser) == 0) {
			found = 1;
			for (int j = i; j < userCount - 1; j++) {
				strcpy(users[j], users[j+1]);
				strcpy(passwords[j], passwords[j+1]);
			}
			userCount--;
			break;
		}
	}
	
	if (!found) {
		setColor(COLOR_RED);
		printf("   用户 %s 不存在！\n", targetUser);
		setColor(COLOR_DEFAULT);
		pause(); return;
	}
	
	// 写回文件
	fp = fopen(USER_FILE, "w");
	if (!fp) {
		setColor(COLOR_RED);
		printf("   无法写入用户文件！\n");
		setColor(COLOR_DEFAULT);
		pause(); return;
	}
	
	for (int i = 0; i < userCount; i++) {
		fprintf(fp, "%s %s\n", users[i], passwords[i]);
	}
	fclose(fp);
	
	setColor(COLOR_GREEN);
	printf("   用户 %s 删除成功！\n", targetUser);
	pause();
}

void applyForReset() {
	char password[100];
	cls();
	printBanner("申请重置次数");
	printf("\n");
	
	printf("  当前剩余次数: %d\n", g_currentUserInfo.remaining_changes);
	printf("  请输入密码验证身份: "); readPwd(password, 1);
	
	if (strcmp(password, g_currentUserInfo.password) != 0) { 
		setColor(COLOR_RED);
		printf("\n   密码验证失败！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	printf("  申请理由: "); 
	char reason[500]; 
	fgets(reason, sizeof(reason), stdin);
	// 移除理由末尾的换行符
	reason[strcspn(reason, "\n")] = '\0';
	
	FILE* fp = fopen("applications.txt", "a");
	if (fp) { 
		fprintf(fp, "%s %s %s\n", g_currentUserInfo.username, g_currentUserInfo.password, reason); 
		fclose(fp); 
		setColor(COLOR_GREEN);
		printf("\n   申请已提交，请等待管理员审批。\n");
		setColor(COLOR_DEFAULT);
	} else {
		setColor(COLOR_RED);
		printf("   无法提交申请！\n");
		setColor(COLOR_DEFAULT);
	}
	pause();
}

void approveApplication() {
	FILE* fp = fopen("applications.txt", "r");
	if (!fp) { 
		setColor(COLOR_YELLOW);
		printf("   暂无待审批申请。\n");
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	char username[100], password[100], reason[500]; 
	int count = 0;
	setColor(COLOR_CYAN);
	printf("  待审批申请列表:\n");
	setColor(COLOR_DEFAULT);
	
	while (fscanf(fp, "%99s %99s %499[^\n]", username, password, reason) >= 2) {
		count++;
		// 处理理由为空的情况
		if (strlen(reason) == 0) {
			strcpy(reason, "（未填写理由）");
		}
		printf("  %d. 用户名: %s\n", count, username);
		printf("     申请理由: %s\n\n", reason);
	}
	
	fclose(fp);
	
	if (count == 0) { 
		setColor(COLOR_YELLOW);
		printf("    无申请记录。\n");
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	printf("  输入要审批的申请序号（0取消）: "); 
	int idx; scanf("%d", &idx);
	if (idx < 1 || idx > count) { 
		setColor(COLOR_YELLOW);
		printf("  已取消。\n");
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	// 重新读取文件并定位到要删除的申请
	fp = fopen("applications.txt", "r");
	FILE* temp = fopen("temp_apps.txt", "w");
	if (!fp || !temp) {
		setColor(COLOR_RED);
		printf("   文件操作失败！\n");
		setColor(COLOR_DEFAULT);
		if (fp) fclose(fp);
		if (temp) fclose(temp);
		pause(); return;
	}
	
	int current = 0; 
	char target_user[100] = "";
	
	while (fscanf(fp, "%99s %99s %499[^\n]", username, password, reason) >= 2) {
		current++;
		if (current == idx) {
			strcpy(target_user, username);
			// 跳过该项，不写入临时文件
		} else {
			// 写入临时文件
			fprintf(temp, "%s %s %s\n", username, password, reason);
		}
	}
	
	fclose(fp); 
	fclose(temp);
	
	remove("applications.txt"); 
	rename("temp_apps.txt", "applications.txt");
	
	if (target_user[0]) { 
		setColor(COLOR_GREEN);
		printf("   申请已批准！请为用户 %s 生成密钥。\n", target_user); 
		setColor(COLOR_DEFAULT);
		generateResetKey(); 
	}
	pause();
}

void generateResetKey() {
	int key = 100000 + rand() % 900000;
	cls();
	printBanner("生成重置密钥");
	printf("\n");
	
	setColor(COLOR_GREEN);
	printf("  ════════════════════════════════════════\n");
	printf("  一次性密钥: %d\n", key);
	printf("  ════════════════════════════════════════\n");
	setColor(COLOR_DEFAULT);
	
	printf("\n  请将此密钥提供给申请用户。\n");
	FILE* fp = fopen("temp_keys.txt", "a");
	if (fp) { fprintf(fp, "%d\n", key); fclose(fp); }
	pause();
}

void useResetKey() {
	char input_key[20];
	cls();
	printBanner("使用密钥重置");
	printf("\n");
	
	printf("  请输入6位密钥: "); scanf("%s", input_key);
	FILE* fp = fopen("temp_keys.txt", "r");
	if (!fp) { 
		setColor(COLOR_RED);
		printf("    密钥无效或已过期！\n");
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	int key, found = 0; FILE* temp = fopen("temp_keys2.txt", "w");
	while (fscanf(fp, "%d", &key) == 1) {
		if (key == atoi(input_key)) found = 1;
		else fprintf(temp, "%d\n", key);
	}
	fclose(fp); fclose(temp);
	remove("temp_keys.txt"); rename("temp_keys2.txt", "temp_keys.txt");
	if (found) {
		g_currentUserInfo.remaining_changes = MAX_CHANGES;
		saveUserInfo(&g_currentUserInfo);
		setColor(COLOR_GREEN);
		printf("   密钥验证成功！重置次数为 %d 次。\n", MAX_CHANGES);
		setColor(COLOR_DEFAULT);
	} else {
		setColor(COLOR_RED);
		printf("   密钥无效或已过期！\n");
		setColor(COLOR_DEFAULT);
	}
	pause();
}

/* ACM简介 */
void showEvent()      { cls(); printBanner("赛事介绍");      showFile(EVENT_FILE);      pause(); }
void showRule()       { cls(); printBanner("比赛规则");       showFile(RULE_FILE);       pause(); }
void showScore()      { cls(); printBanner("评分细则");       showFile(SCORE_FILE);      pause(); }
void showStructure()  { cls(); printBanner("赛事构成");       showFile(STRUCTURE_FILE);  pause(); }

/*计算字符串显示宽度*/
int getDisplayWidth(const char* str) {
	int width = 0;
	while (*str) {
		if ((unsigned char)*str > 127) { // 中文字符
			width += 2;
			str++; // 跳过第二个字节
		} else {
			width += 1;
		}
		str++;
	}
	return width;
}

/*辅助函数：填充空格到指定宽度*/
void printPadded(const char* str, int targetWidth) {
	int currentWidth = getDisplayWidth(str);
	int pad = targetWidth - currentWidth;
	if (pad < 0) pad = 0; // 如果超出宽度
	
	printf("%s", str);
	for (int i = 0; i < pad; i++) printf(" ");
}

void showWinners() {
	cls();
	setColor(COLOR_YELLOW);
	printf("╔══════════════════════════════════════════════════════════════════════╗\n");
	printf("║                          历届获奖名单                                ║\n");
	printf("╠══════════════════════════════════════════════════════════════════════╣\n");
	printf("║ 年份 │ 地点        │ 冠军           │ 国家   │ 队员           │ 教练    ║\n");
	printf("╚══════════════════════════════════════════════════════════════════════╝\n");
	setColor(COLOR_DEFAULT);
	
	FILE* fp = fopen(WINNER_FILE, "r");
	if (!fp) { 
		setColor(COLOR_RED);
		printf("    历届获奖数据文件缺失！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	char line[MAX_LINE];
	setColor(COLOR_CYAN);
	while (fgets(line, sizeof(line), fp)) {
		line[strcspn(line, "\n")] = '\0'; // 移除换行符
		
		int year; 
		char loc[50], champ[50], country[20], members[100], coach[50];
		
		// 健壮化解析方式
		if (sscanf(line, "%d %49s %49s %19s %99[^ ] %49s", &year, loc, champ, country, members, coach) == 6) {
			// 动态格式化输出，确保对齐
			printf("║ ");
			
			// 年份（固定4字符）
			printf("%-4d │ ", year);
			
			// 地点（目标宽度8，但用空格手动对齐）
			printPadded(loc, 10); printf(" │ ");
			
			// 冠军（目标宽度12）
			printPadded(champ, 14); printf(" │ ");
			
			// 国家（目标宽度6）
			printPadded(country, 6); printf(" │ ");
			
			// 队员（目标宽度15）
			printPadded(members, 15); printf(" │ ");
			
			// 教练（剩余宽度）
			printf("%s", coach);
			
			printf(" ║\n");
		}
	}
	
	setColor(COLOR_YELLOW);
	printf("╚══════════════════════════════════════════════════════════════════════╝\n");
	setColor(COLOR_DEFAULT);
	fclose(fp);
	
	printf("\n  【1】 开启检索  【2】 返回上级\n  ");
	printf("请选择：");
	int opt; scanf("%d", &opt);
	if (opt == 1) searchWinner();
}

void searchWinner() {
	char keyword[100];
	printf("\n  输入关键词（大学/地点/队员等）: "); scanf("%s", keyword);
	
	FILE* fp = fopen(WINNER_FILE, "r");
	if (!fp) { 
		setColor(COLOR_RED);
		printf("   数据文件缺失！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	char line[MAX_LINE]; 
	int found = 0;
	
	setColor(COLOR_GREEN);
	printf("\n  ════════════════════════════════════════════════════════════\n");
	printf("  搜索结果：\n");
	printf("  ════════════════════════════════════════════════════════════\n");
	setColor(COLOR_DEFAULT);
	
	while (fgets(line, sizeof(line), fp)) {
		line[strcspn(line, "\n")] = '\0';
		
		if (strstr(line, keyword)) {
			int year; 
			char loc[50], champ[50], country[20], members[100], coach[50];
			
			if (sscanf(line, "%d %49s %49s %19s %99[^ ] %49s", &year, loc, champ, country, members, coach) == 6) {
				printf("  → %d年 %s 冠军: %s (%s)\n", year, loc, champ, members);
				found = 1;
			}
		}
	}
	
	fclose(fp); 
	
	if (!found) {
		setColor(COLOR_YELLOW);
		printf("\n    未找到匹配项。\n");
		setColor(COLOR_DEFAULT);
	}
	
	pause();
}

/*题库管理*/

// 题目结构体
typedef struct {
	int id;
	char title[200];
	char difficulty[50];
	char algorithm[200];
	char description[500];
	char sample_in[200];
	char sample_out[200];
	char answer_file[100];
	char test_file[100];
	char solution_file[100];
} Question;

// 从共享文件中删除指定ID的块
void deleteBlockFromFile(const char* filename, int id) {
	char filepath[500], temppath[500];
	sprintf(filepath, "%s\\%s", QUESTION_FOLDER, filename);
	sprintf(temppath, "%s.temp", filepath);
	
	FILE* fp = fopen(filepath, "r");
	if (!fp) return;
	
	FILE* fp_temp = fopen(temppath, "w");
	if (!fp_temp) { fclose(fp); return; }
	
	char line[MAX_LINE];
	int skip_block = 0;
	
	while (fgets(line, sizeof(line), fp)) {
		if (strncmp(line, "[ID:", 4) == 0) {
			int current_id = -1;
			sscanf(line, "[ID:%d]", &current_id);
			skip_block = (current_id == id) ? 1 : 0;
		}
		
		if (!skip_block) {
			fputs(line, fp_temp);
		}
	}
	
	fclose(fp);
	fclose(fp_temp);
	
	remove(filepath);
	rename(temppath, filepath);
}

// 从索引文件加载所有题目
int loadQuestions(Question questions[], int max_count) {
	FILE* fp = fopen(QUESTION_FILE, "r");
	if (!fp) return 0;
	
	char line[MAX_LINE];
	int count = 0;
	int in_block = 0;
	
	while (fgets(line, sizeof(line), fp) && count < max_count) {
		line[strcspn(line, "\n")] = 0;
		
		if (strncmp(line, "[ID:", 4) == 0) {
			if (in_block) count++;
			in_block = 1;
			sscanf(line, "[ID:%d]", &questions[count].id);
		} else if (in_block) {
			if (sscanf(line, "标题: %199[^\n]", questions[count].title) == 1) continue;
			if (sscanf(line, "难度: %49[^\n]", questions[count].difficulty) == 1) continue;
			if (sscanf(line, "算法: %199[^\n]", questions[count].algorithm) == 1) continue;
			if (sscanf(line, "题干: %499[^\n]", questions[count].description) == 1) continue;
			if (sscanf(line, "样例输入: %199[^\n]", questions[count].sample_in) == 1) continue;
			if (sscanf(line, "样例输出: %199[^\n]", questions[count].sample_out) == 1) continue;
			if (sscanf(line, "答案文件: %99s", questions[count].answer_file) == 1) continue;
			if (sscanf(line, "检测点文件: %99s", questions[count].test_file) == 1) continue;
			if (sscanf(line, "题解文件: %99s", questions[count].solution_file) == 1) continue;
		}
	}
	if (in_block) count++;
	fclose(fp);
	return count;
}

// 保存所有题目到索引文件
void saveQuestions(Question questions[], int count) {
	FILE* fp = fopen(QUESTION_FILE, "w");
	if (!fp) { 
		setColor(COLOR_RED);
		printf("   无法写入题库索引！\n"); 
		setColor(COLOR_DEFAULT);
		return; 
	}
	
	for (int i = 0; i < count; i++) {
		fprintf(fp, "[ID:%d]\n", questions[i].id);
		fprintf(fp, "标题: %s\n", questions[i].title);
		fprintf(fp, "难度: %s\n", questions[i].difficulty);
		fprintf(fp, "算法: %s\n", questions[i].algorithm);
		fprintf(fp, "题干: %s\n", questions[i].description);
		fprintf(fp, "样例输入: %s\n", questions[i].sample_in);
		fprintf(fp, "样例输出: %s\n", questions[i].sample_out);
		fprintf(fp, "答案文件: %s\n", questions[i].answer_file);
		fprintf(fp, "检测点文件: %s\n", questions[i].test_file);
		fprintf(fp, "题解文件: %s\n", questions[i].solution_file);
		fprintf(fp, "---\n");
	}
	fclose(fp);
}

void displayQuestions() {
	while (1) {
		cls();
		setColor(COLOR_CYAN);
		printf("╔════════════════════════════════════════════════════════════╗\n");
		printf("║                      ACM题库列表                           ║\n");
		printf("╠════════════════════════════════════════════════════════════╣\n");
		setColor(COLOR_DEFAULT);
		
		Question questions[MAX_ITEMS];
		int count = loadQuestions(questions, MAX_ITEMS);
		
		if (count == 0) {
			setColor(COLOR_YELLOW);
			printf("║  题库为空！                                                 ║\n");
			setColor(COLOR_DEFAULT);
			printf("╚════════════════════════════════════════════════════════════╝\n");
			pause(); return;
		}
		
		// 显示简略列表
		setColor(COLOR_GREEN);
		printf("║  ID   | 标题                 | 难度     | 算法           ║\n");
		printf("╠═══════╪══════════════════════╪══════════╪════════════════╣\n");
		setColor(COLOR_DEFAULT);
		
		for (int i = 0; i < count; i++) {
			printf("║  %-4d │ %-20s │ ", questions[i].id, questions[i].title);
			setColor(COLOR_YELLOW);
			printf("%-8s", questions[i].difficulty);
			setColor(COLOR_DEFAULT);
			printf(" │ %-15s║\n", questions[i].algorithm);
		}
		
		printf("╚════════════════════════════════════════════════════════════╝\n");
		printf("  共 %d 道题目\n\n", count);
		
		printf("  输入题目ID查看详细题干（0返回上级）: ");
		int query_id; scanf("%d", &query_id);
		if (query_id == 0) break;  // 返回上级菜单
		
		// 查找并显示题目详情
		int found = -1;
		for (int i = 0; i < count; i++) {
			if (questions[i].id == query_id) { found = i; break; }
		}
		
		if (found == -1) {
			setColor(COLOR_RED);
			printf("  未找到ID为 %d 的题目！\n", query_id);
			setColor(COLOR_DEFAULT);
			pause();
		} else {
			cls();
			setColor(COLOR_MAGENTA);
			printf("╔════════════════════════════════════════════════════════════╗\n");
			printf("║  题目详情 - ID: %-4d                                      ║\n", query_id);
			printf("╚════════════════════════════════════════════════════════════╝\n");
			setColor(COLOR_DEFAULT);
			
			printf("\n  标题: %s\n", questions[found].title);
			printf("  难度: %s | 算法: %s\n\n", questions[found].difficulty, questions[found].algorithm);
			
			setColor(COLOR_CYAN);
			printf("  完整题干:\n");
			setColor(COLOR_DEFAULT);
			printf("  %s\n", questions[found].description);
			
			if (questions[found].sample_in[0]) {
				setColor(COLOR_YELLOW);
				printf("\n  样例输入:\n");
				setColor(COLOR_DEFAULT);
				printf("  %s\n", questions[found].sample_in);
				
				setColor(COLOR_YELLOW);
				printf("  样例输出:\n");
				setColor(COLOR_DEFAULT);
				printf("  %s\n", questions[found].sample_out);
			}
			pause();
		}
	}
}

void searchQuestion() {
	int mode; cls();
	setColor(COLOR_CYAN);
	printf("╔════════════════════════════════════════════════════════════╗\n");
	printf("║                      查询赛题                              ║\n");
	printf("╚════════════════════════════════════════════════════════════╝\n");
	setColor(COLOR_DEFAULT);
	
	printf("\n  1. 单条件查询\n  2. 高级多条件查询\n  请选择模式（1-2）: ");
	scanf("%d", &mode); while (getchar() != '\n');
	
	char id_str[50] = "", title[200] = "", difficulty[50] = ""; 
	char algorithm[200] = "", desc_keyword[200] = "";
	int search_id = -1;
	
	if (mode == 1) {
		printf("\n  选择查询字段：\n  1. 题目ID  2. 标题  3. 难度  4. 算法  5. 题干\n  请选择（1-5）: ");
		int field; scanf("%d", &field); while (getchar() != '\n');
		printf("  请输入关键词: "); char temp[500];
		fgets(temp, sizeof(temp), stdin);
		temp[strcspn(temp, "\n")] = '\0';
		switch(field) {
			case 1: strcpy(id_str, temp); break;
			case 2: strcpy(title, temp); break;
			case 3: strcpy(difficulty, temp); break;
			case 4: strcpy(algorithm, temp); break;
			case 5: strcpy(desc_keyword, temp); break;
		}
	} else if (mode == 2) {
		printf("\n  高级查询（直接回车跳过）:\n");
		printf("  题目ID: "); fgets(id_str, sizeof(id_str), stdin);
		if (id_str[0] != '\n') id_str[strcspn(id_str, "\n")] = '\0'; else id_str[0] = '\0';
		printf("  标题: "); fgets(title, sizeof(title), stdin);
		if (title[0] != '\n') title[strcspn(title, "\n")] = '\0'; else title[0] = '\0';
		printf("  难度: "); fgets(difficulty, sizeof(difficulty), stdin);
		if (difficulty[0] != '\n') difficulty[strcspn(difficulty, "\n")] = '\0'; else difficulty[0] = '\0';
		printf("  算法: "); fgets(algorithm, sizeof(algorithm), stdin);
		if (algorithm[0] != '\n') algorithm[strcspn(algorithm, "\n")] = '\0'; else algorithm[0] = '\0';
		printf("  题干关键词: "); fgets(desc_keyword, sizeof(desc_keyword), stdin);
		if (desc_keyword[0] != '\n') desc_keyword[strcspn(desc_keyword, "\n")] = '\0'; else desc_keyword[0] = '\0';
	} else { 
		setColor(COLOR_RED);
		printf("   无效选择！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	if (id_str[0]) search_id = atoi(id_str);
	
	Question questions[MAX_ITEMS];
	int count = loadQuestions(questions, MAX_ITEMS);
	int match_count = 0;
	
	for (int i = 0; i < count; i++) {
		int match = 1;
		if (search_id > 0 && questions[i].id != search_id) match = 0;
		if (title[0] && !strstr(questions[i].title, title)) match = 0;
		if (difficulty[0] && !strstr(questions[i].difficulty, difficulty)) match = 0;
		if (algorithm[0] && !strstr(questions[i].algorithm, algorithm)) match = 0;
		if (desc_keyword[0] && !strstr(questions[i].description, desc_keyword)) match = 0;
		
		if (match) {
			setColor(COLOR_GREEN);
			printf("\n  ═══════════════════ 题目 ID: %d ═══════════════════\n", questions[i].id);
			setColor(COLOR_DEFAULT);
			printf("  标题: %s\n  难度: %s | 算法: %s\n", questions[i].title, questions[i].difficulty, questions[i].algorithm);
			printf("  题干: %.100s%s\n", questions[i].description, strlen(questions[i].description) > 100 ? "..." : "");
			printf("  答案文件: %s\n  检测点文件: %s\n  题解文件: %s\n", 
				   questions[i].answer_file, questions[i].test_file, questions[i].solution_file);
			match_count++;
		}
	}
	
	if (match_count == 0) {
		setColor(COLOR_YELLOW);
		printf("\n   未找到匹配的赛题。\n");
		setColor(COLOR_DEFAULT);
	} else {
		printf("\n  共找到 %d 道赛题。\n", match_count);
	}
	pause();
}

void addQuestion() {
	if (!checkAndConsumeChange()) return;
	
	Question q;
	cls();
	printBanner("添加新赛题");
	printf("\n");
	
	printf("  ID: "); scanf("%d", &q.id);
	printf("  标题: "); scanf(" %[^\n]", q.title);
	printf("  难度: "); scanf("%s", q.difficulty);
	printf("  算法点: "); scanf(" %[^\n]", q.algorithm);
	printf("  题干: "); scanf(" %[^\n]", q.description);
	printf("  样例输入: "); scanf(" %[^\n]", q.sample_in);
	printf("  样例输出: "); scanf(" %[^\n]", q.sample_out);
	printf("  答案文件名(如答案8.c): "); scanf("%s", q.answer_file);
	strcpy(q.test_file, "检测点.txt");
	strcpy(q.solution_file, "解析.txt");
	
	Question questions[MAX_ITEMS];
	int count = loadQuestions(questions, MAX_ITEMS);
	
	for (int i = 0; i < count; i++) {
		if (questions[i].id == q.id) {
			setColor(COLOR_RED);
			printf("\n   错误：ID %d 已存在！\n", q.id);
			setColor(COLOR_DEFAULT);
			pause(); return;
		}
	}
	
	setColor(COLOR_YELLOW);
	printf("\n  ════════════════════════════════════════════════════════\n");
	printf("  请输入答案代码（输入%%%%结束）:\n");
	setColor(COLOR_DEFAULT);
	char answer_code[5000] = {0}, buffer[MAX_LINE];
	while (getchar() != '\n');
	while (1) {
		if (!fgets(buffer, sizeof(buffer), stdin)) break;
		if (strcmp(buffer, "%%%%\n") == 0) break;
		if (strlen(answer_code) + strlen(buffer) < sizeof(answer_code) - 1) 
			strcat(answer_code, buffer);
	}
	
	char answer_path[500];
	sprintf(answer_path, "%s\\%s", QUESTION_FOLDER, q.answer_file);
	FILE* fp_answer = fopen(answer_path, "w");
	if (fp_answer) {
		fputs(answer_code, fp_answer);
		fclose(fp_answer);
		setColor(COLOR_GREEN);
		printf("   答案文件已创建: %s\n", q.answer_file);
		setColor(COLOR_DEFAULT);
	} else {
		setColor(COLOR_RED);
		printf("   警告：无法创建答案文件。\n");
		setColor(COLOR_DEFAULT);
	}
	
	setColor(COLOR_YELLOW);
	printf("\n  ════════════════════════════════════════════════════════\n");
	printf("  请输入检测点内容（格式[input]和[output]，输入%%%%结束）:\n");
	setColor(COLOR_DEFAULT);
	char test_content[5000] = {0};
	while (1) {
		if (!fgets(buffer, sizeof(buffer), stdin)) break;
		if (strcmp(buffer, "%%%%\n") == 0) break;
		if (strlen(test_content) + strlen(buffer) < sizeof(test_content) - 1) 
			strcat(test_content, buffer);
	}
	
	char test_path[500];
	sprintf(test_path, "%s\\%s", QUESTION_FOLDER, q.test_file);
	FILE* fp_test = fopen(test_path, "a");
	if (fp_test) {
		fprintf(fp_test, "[ID:%d]\n%s", q.id, test_content);
		fclose(fp_test);
		setColor(COLOR_GREEN);
		printf("   检测点已添加到: %s\n", q.test_file);
		setColor(COLOR_DEFAULT);
	} else {
		setColor(COLOR_RED);
		printf("   警告：无法写入检测点文件。\n");
		setColor(COLOR_DEFAULT);
	}
	
	setColor(COLOR_YELLOW);
	printf("\n  ════════════════════════════════════════════════════════\n");
	printf("  请输入题解内容（需包含[answer]和[分析]部分，输入%%%%结束）:\n");
	setColor(COLOR_DEFAULT);
	char solution_content[5000] = {0};
	while (1) {
		if (!fgets(buffer, sizeof(buffer), stdin)) break;
		if (strcmp(buffer, "%%%%\n") == 0) break;
		if (strlen(solution_content) + strlen(buffer) < sizeof(solution_content) - 1) 
			strcat(solution_content, buffer);
	}
	
	char solution_path[500];
	sprintf(solution_path, "%s\\%s", QUESTION_FOLDER, q.solution_file);
	FILE* fp_solution = fopen(solution_path, "a");
	if (fp_solution) {
		fprintf(fp_solution, "[ID:%d]\n%s", q.id, solution_content);
		fclose(fp_solution);
		setColor(COLOR_GREEN);
		printf("   题解已添加到: %s\n", q.solution_file);
		setColor(COLOR_DEFAULT);
	} else {
		setColor(COLOR_RED);
		printf("   警告：无法写入题解文件。\n");
		setColor(COLOR_DEFAULT);
	}
	
	questions[count] = q;
	count++;
	saveQuestions(questions, count);
	
	setColor(COLOR_GREEN);
	printf("\n   赛题添加成功！\n");
	setColor(COLOR_DEFAULT);
	pause();
}

void deleteQuestion() {
	if (!checkAndConsumeChange()) return;
	
	int id;
	cls();
	printBanner("删除赛题");
	printf("\n");
	
	printf("  输入要删除的赛题ID: "); scanf("%d", &id);
	
	Question questions[MAX_ITEMS];
	int count = loadQuestions(questions, MAX_ITEMS);
	
	int found = -1;
	for (int i = 0; i < count; i++) {
		if (questions[i].id == id) { found = i; break; }
	}
	
	if (found == -1) {
		setColor(COLOR_RED);
		printf("   未找到ID为 %d 的赛题！\n", id);
		setColor(COLOR_DEFAULT);
		pause(); return;
	}
	
	printf("  确认删除题目 [%d] %s 吗？(Y/N): ", questions[found].id, questions[found].title);
	char confirm; scanf(" %c", &confirm);
	if (confirm != 'Y' && confirm != 'y') { 
		setColor(COLOR_YELLOW);
		printf("  已取消删除。\n");
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	char answer_path[500];
	sprintf(answer_path, "%s\\%s", QUESTION_FOLDER, questions[found].answer_file);
	if (remove(answer_path) == 0) {
		setColor(COLOR_GREEN);
		printf("   已删除答案文件: %s\n", questions[found].answer_file);
		setColor(COLOR_DEFAULT);
	} else {
		setColor(COLOR_YELLOW);
		printf("   答案文件不存在或删除失败。\n");
		setColor(COLOR_DEFAULT);
	}
	
	deleteBlockFromFile(questions[found].test_file, id);
	printf("  已从检测点文件移除相关内容。\n");
	
	deleteBlockFromFile(questions[found].solution_file, id);
	printf("  已从解析文件移除相关内容。\n");
	
	for (int i = found; i < count - 1; i++) questions[i] = questions[i+1];
	count--;
	
	saveQuestions(questions, count);
	setColor(COLOR_GREEN);
	printf("   赛题删除成功！\n");
	setColor(COLOR_DEFAULT);
	pause();
}

/*竞赛训练*/
void solveQuestion() {
	int id; 
	cls();
	printBanner("赛题题解");
	printf("\n");
	
	printf("  输入赛题ID: "); scanf("%d", &id);
	
	Question questions[MAX_ITEMS];
	int count = loadQuestions(questions, MAX_ITEMS);
	
	int found = -1;
	for (int i = 0; i < count; i++) {
		if (questions[i].id == id) { found = i; break; }
	}
	
	if (found == -1) { 
		setColor(COLOR_RED);
		printf("    未找到该赛题！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	if (questions[found].answer_file[0]) {
		char answer_path[500];
		sprintf(answer_path, "%s\\%s", QUESTION_FOLDER, questions[found].answer_file);
		setColor(COLOR_GREEN);
		printf("\n  ═══════════════════ 参考答案代码 ═══════════════════\n");
		setColor(COLOR_DEFAULT);
		showFile(answer_path);
	} else {
		setColor(COLOR_YELLOW);
		printf("   该题目暂无答案文件！\n");
		setColor(COLOR_DEFAULT);
	}
	
	pause();
}

void runTestCases() {
	int id;
	cls();
	printBanner("检测点答题");
	printf("\n");
	
	printf("  输入赛题ID: "); scanf("%d", &id);
	
	Question questions[MAX_ITEMS];
	int count = loadQuestions(questions, MAX_ITEMS);
	
	int found = -1;
	for (int i = 0; i < count; i++) if (questions[i].id == id) { found = i; break; }
	if (found == -1) { 
		setColor(COLOR_RED);
		printf("    未找到该赛题！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	if (!questions[found].test_file[0]) {
		setColor(COLOR_YELLOW);
		printf("    该题目暂无检测点文件！\n");
		setColor(COLOR_DEFAULT);
		pause(); return;
	}
	
	// 显示题目详情
	setColor(COLOR_CYAN);
	printf("\n  ═══════════════════ 题目详情 ═══════════════════\n");
	setColor(COLOR_DEFAULT);
	printf("  标题: %s\n", questions[found].title);
	printf("  难度: %s | 算法: %s\n", questions[found].difficulty, questions[found].algorithm);
	printf("  题干: %s\n", questions[found].description);
	
	if (questions[found].sample_in[0]) {
		setColor(COLOR_YELLOW);
		printf("\n  样例输入:\n");
		setColor(COLOR_DEFAULT);
		printf("  %s\n", questions[found].sample_in);
		
		setColor(COLOR_YELLOW);
		printf("  样例输出:\n");
		setColor(COLOR_DEFAULT);
		printf("  %s\n", questions[found].sample_out);
	}
	
	// 读取检测点
	char test_path[500];
	sprintf(test_path, "%s\\%s", QUESTION_FOLDER, questions[found].test_file);
	
	FILE* fp_test = fopen(test_path, "r");
	if (!fp_test) { 
		setColor(COLOR_RED);
		printf("   检测点文件不存在: %s\n", test_path); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	// 定位到目标ID的检测点块
	char line[MAX_LINE];
	int in_target_block = 0;
	int found_test_block = 0;
	
	while (fgets(line, sizeof(line), fp_test)) {
		if (strncmp(line, "[ID:", 4) == 0) {
			int current_id = -1;
			sscanf(line, "[ID:%d]", &current_id);
			if (current_id == id) {
				in_target_block = 1;
				found_test_block = 1;
				break;
			} else {
				in_target_block = 0;
			}
		}
	}
	
	if (!found_test_block) {
		setColor(COLOR_YELLOW);
		printf("   未找到ID为 %d 的检测点数据！\n", id);
		setColor(COLOR_DEFAULT);
		fclose(fp_test);
		pause(); return;
	}
	
	// 输入代码
	setColor(COLOR_YELLOW);
	printf("\n  ════════════════════════════════════════════════════════\n");
	printf("  请输入你的C语言代码（输入%%%%结束）:\n");
	setColor(COLOR_DEFAULT);
	char user_code[5000] = {0}, buffer[MAX_LINE];
	while (1) {
		if (!fgets(buffer, sizeof(buffer), stdin)) break;
		if (strcmp(buffer, "%%%%\n") == 0) break;
		if (strlen(user_code) + strlen(buffer) < sizeof(user_code) - 1) strcat(user_code, buffer);
	}
	
	FILE* fp_code = fopen("temp_user.c", "w");
	if (!fp_code) { 
		setColor(COLOR_RED);
		printf("    无法创建代码文件！\n"); 
		setColor(COLOR_DEFAULT);
		fclose(fp_test);
		pause(); return; 
	}
	fputs(user_code, fp_code); fclose(fp_code);
	
	setColor(COLOR_CYAN);
	printf("\n  【编译代码...】\n");
	setColor(COLOR_DEFAULT);
	if (system("gcc temp_user.c -o temp_user.exe 2> compile_err.log") != 0) {//0成功，其它失败
		setColor(COLOR_RED);
		printf("    编译失败！请查看 compile_err.log\n");
		setColor(COLOR_DEFAULT);
		remove("temp_user.c");
		fclose(fp_test);
		pause(); return;
	} else {
		setColor(COLOR_GREEN);
		printf("    编译成功！\n");
		setColor(COLOR_DEFAULT);
	}
	
	// 运行检测点
	setColor(COLOR_CYAN);
	printf("\n  【运行检测点...】\n");
	setColor(COLOR_DEFAULT);
	int test_count = 0, passed = 0;
	char input[500] = {0}, expected[500] = {0};
	int reading_input = 0, reading_output = 0;
	
	while (fgets(line, sizeof(line), fp_test)) {
		if (strncmp(line, "[ID:", 4) == 0) break;  // 遇到下一个ID块就停止
		
		if (strstr(line, "[input]")) { reading_input = 1; reading_output = 0; input[0] = '\0'; }
		else if (strstr(line, "[output]")) { reading_input = 0; reading_output = 1; expected[0] = '\0'; }
		else if (reading_input) strcat(input, line);
		else if (reading_output) {
			strcat(expected, line);
			test_count++;
			setColor(COLOR_MAGENTA);
			printf("\n  ─── 检测点 %d ───\n", test_count);
			setColor(COLOR_DEFAULT);
			FILE* fp_input = fopen("temp_input.txt", "w");
			fputs(input, fp_input); fclose(fp_input);
			system("temp_user.exe < temp_input.txt > temp_output.txt");
			FILE* fp_out = fopen("temp_output.txt", "r");
			char actual[500] = {0}; 
			fgets(actual, sizeof(actual), fp_out); 
			fclose(fp_out);
			
			// 移除 actual 和 expected 中的 \r 和 \n
			char *p;
			while ((p = strchr(actual, '\r')) != NULL) *p = '\0';
			while ((p = strchr(actual, '\n')) != NULL) *p = '\0';
			while ((p = strchr(expected, '\r')) != NULL) *p = '\0';
			while ((p = strchr(expected, '\n')) != NULL) *p = '\0';
			
			// 移除末尾空格
			size_t alen = strlen(actual);
			while (alen > 0 && actual[alen-1] == ' ') { actual[alen-1] = '\0'; alen--; }
			size_t elen = strlen(expected);
			while (elen > 0 && expected[elen-1] == ' ') { expected[elen-1] = '\0'; elen--; }
			
			// 比较并显示带引号的结果，便于调试
			if (strcmp(actual, expected) == 0) { 
				setColor(COLOR_GREEN);
				printf("   通过！\n");
				setColor(COLOR_DEFAULT);
				passed++; 
			}
			else { 
				setColor(COLOR_RED);
				printf("   失败！\n  期望值: '%s'\n  实际值: '%s'\n", expected, actual); 
				setColor(COLOR_DEFAULT);
			}
			remove("temp_input.txt"); remove("temp_output.txt");
		}
	}
	fclose(fp_test);
	
	setColor(COLOR_CYAN);
	printf("\n  ═══════════════════ 检测结果 ═══════════════════\n");
	setColor(COLOR_DEFAULT);
	printf("  共 %d 个检测点，", test_count);
	setColor(COLOR_GREEN);
	printf("通过 %d 个\n", passed);
	setColor(COLOR_DEFAULT);
	if (passed == test_count) {
		setColor(COLOR_GREEN);
		printf("   全部通过！答案正确\n");
		setColor(COLOR_DEFAULT);
	} else {
		setColor(COLOR_YELLOW);
		printf("   部分失败，请检查代码\n");
		setColor(COLOR_DEFAULT);
	}
	remove("temp_user.c"); remove("temp_user.exe"); remove("compile_err.log");
	pause();
}

void runStandardAnswer() {
	int id;
	cls();
	printBanner("运行标准答案");
	printf("\n");
	
	printf("  输入赛题ID: "); scanf("%d", &id);
	
	Question questions[MAX_ITEMS];
	int count = loadQuestions(questions, MAX_ITEMS);
	
	int found = -1;
	for (int i = 0; i < count; i++) if (questions[i].id == id) { found = i; break; }
	if (found == -1) { 
		setColor(COLOR_RED);
		printf("   未找到该赛题！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	char answer_file[500];
	sprintf(answer_file, "%s\\%s", QUESTION_FOLDER, questions[found].answer_file);
	if (access(answer_file, 0) != 0) {
		setColor(COLOR_RED);
		printf("   标准答案文件不存在：%s\n", answer_file); 
		setColor(COLOR_DEFAULT);
		pause(); return;
	}
	
	char exe_file[500];
	sprintf(exe_file, "%s\\%d_std.exe", QUESTION_FOLDER, id);
	setColor(COLOR_CYAN);
	printf("\n  【编译标准答案...】\n");
	setColor(COLOR_DEFAULT);
	char cmd[1000];
	sprintf(cmd, "gcc -std=c99 \"%s\" -o \"%s\" 2> \"%s\\compile_std.log\"", answer_file, exe_file, QUESTION_FOLDER);
	if (system(cmd) != 0) { 
		setColor(COLOR_RED);
		printf("   编译失败！请查看 %s\\compile_std.log\n", QUESTION_FOLDER); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	} else {
		setColor(COLOR_GREEN);
		printf("   编译成功！\n");
		setColor(COLOR_DEFAULT);
	}
	
	if (questions[found].test_file[0]) {
		char test_path[500];
		sprintf(test_path, "%s\\%s", QUESTION_FOLDER, questions[found].test_file);
		setColor(COLOR_CYAN);
		printf("\n  ═══════════════════ 运行检测点 ═══════════════════\n");
		setColor(COLOR_DEFAULT);
		sprintf(cmd, "\"%s\" < \"%s\"", exe_file, test_path);
		system(cmd);
	} else {
		setColor(COLOR_CYAN);
		printf("\n  ═══════════════════ 标准答案输出 ═══════════════════\n");
		setColor(COLOR_DEFAULT);
		system(exe_file);
	}
	remove(exe_file);
	pause();
}

void runUserCode() {
	int id;
	cls();
	printBanner("智能体训练");
	printf("\n");
	
	printf("  输入赛题ID: "); scanf("%d", &id);
	
	Question questions[MAX_ITEMS];
	int count = loadQuestions(questions, MAX_ITEMS);
	
	int found = -1;
	for (int i = 0; i < count; i++) if (questions[i].id == id) { found = i; break; }
	if (found == -1) { 
		setColor(COLOR_RED);
		printf("    未找到该赛题！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	setColor(COLOR_CYAN);
	printf("\n  ═══════════════════ 题目描述 ═══════════════════\n");
	setColor(COLOR_DEFAULT);
	printf("  %s\n", questions[found].description);
	if (questions[found].sample_in[0]) {
		setColor(COLOR_YELLOW);
		printf("\n  样例输入:\n");
		setColor(COLOR_DEFAULT);
		printf("  %s\n", questions[found].sample_in);
		setColor(COLOR_YELLOW);
		printf("  样例输出:\n");
		setColor(COLOR_DEFAULT);
		printf("  %s\n", questions[found].sample_out);
	}
	
	setColor(COLOR_YELLOW);
	printf("\n  ═══════════════════ 请输入你的C语言代码（输入%%%%结束） ═══════════════════\n");
	setColor(COLOR_DEFAULT);
	char user_code[5000] = {0}, buffer[MAX_LINE];
	while (1) {
		if (!fgets(buffer, sizeof(buffer), stdin)) break;
		if (strcmp(buffer, "%%%%\n") == 0) break;
		if (strlen(user_code) + strlen(buffer) < sizeof(user_code) - 1) strcat(user_code, buffer);
	}
	
	char reference[2000] = "";
	if (questions[found].solution_file[0]) {
		char ref_path[500];
		sprintf(ref_path, "%s\\%s", QUESTION_FOLDER, questions[found].solution_file);
		FILE* ref_fp = fopen(ref_path, "r");
		if (ref_fp) {
			char buf[MAX_LINE];
			while (fgets(buf, sizeof(buf), ref_fp))
				if (strlen(reference) + strlen(buf) < sizeof(reference) - 1) strcat(reference, buf);
			fclose(ref_fp);
		}
	}
	
	FILE* prompt_fp = fopen("ai_prompt.txt", "w");
	if (!prompt_fp) { 
		setColor(COLOR_RED);
		printf("    无法创建 prompt 文件！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	fprintf(prompt_fp,
			"你是一位ACM竞赛教练，请严格根据以下任务评审用户代码：\n"
			"1. 简述题目核心要求（输入格式、输出格式、算法目标）。\n"
			"2. 分析用户代码是否满足题目：\n"
			"   - 能否通过样例？\n"
			"   - 是否处理边界情况？\n"
			"   - 是否有逻辑错误、数组越界等问题？\n"
			"3. 若正确：肯定优点 + 1-2条优化建议。\n"
			"4. 若错误：明确指出错误位置、原因、修复方法。\n"
			"5. 用一句鼓励的话结尾。\n"
			"6. 禁止：不要重写完整代码，不要说'我会这样写'。\n\n"
			"题目描述：%s\n", questions[found].description);//AI设定，让AI先读取这个，明确自己的身份，这样就能按要求回答了
	
	if (questions[found].sample_in[0]) fprintf(prompt_fp, "样例输入：%s\n样例输出：%s\n", questions[found].sample_in, questions[found].sample_out);
	if (reference[0]) fprintf(prompt_fp, "参考解析：\n%s\n", reference);
	fprintf(prompt_fp, "用户代码：\n%s\n", user_code);
	fclose(prompt_fp);
	
	cls();
	setColor(COLOR_GREEN);
	printf("\n  ════════════════════════════════════════════════════════\n");
	printf("       AI 教练正在分析你的代码...\n");
	printf("  ════════════════════════════════════════════════════════\n\n");
	setColor(COLOR_DEFAULT);
	system("ollama run qwen2.5-coder:7b < ai_prompt.txt");//这里我们用的是qwen2.5-coder:7b模型，如果要云版本的更高速的模型，可以换。
	remove("ai_prompt.txt");
	pause();
}

void showQuestionSolution() {
	int id;
	cls();
	printBanner("题目解析");
	printf("\n");
	
	printf("  输入赛题ID: "); scanf("%d", &id);
	
	Question questions[MAX_ITEMS];
	int count = loadQuestions(questions, MAX_ITEMS);
	
	int found = -1;
	for (int i = 0; i < count; i++) {
		if (questions[i].id == id) { found = i; break; }
	}
	
	if (found == -1) { 
		setColor(COLOR_RED);
		printf("    未找到该赛题！\n"); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	char solution_path[500];
	sprintf(solution_path, "%s\\%s", QUESTION_FOLDER, questions[found].solution_file);
	FILE* fp_sol = fopen(solution_path, "r");
	
	if (!fp_sol) { 
		setColor(COLOR_RED);
		printf("   解析文件不存在: %s\n", solution_path); 
		setColor(COLOR_DEFAULT);
		pause(); return; 
	}
	
	char line[MAX_LINE];
	int in_target_block = 0;
	int in_answer = 0, in_analysis = 0;
	int found_block = 0;
	int content_line_count = 0;
	
	printf("  正在查找ID为 %d 的解析内容...\n", id);
	
	while (fgets(line, sizeof(line), fp_sol)) {
		// 标准化行：移除回车符和换行符
		size_t len = strlen(line);
		while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
			line[--len] = '\0';
		}
		
		// 跳过块外的空行
		if (len == 0 && !in_target_block) continue;
		
		// 调试，显示标记行
		if (len > 0 && line[0] == '[') {
			printf("  [调试] 标记: '%s'\n", line);
		}
		
		// 检查ID块
		if (strncmp(line, "[ID:", 4) == 0) {
			int current_id = -1;
			sscanf(line, "[ID:%d]", &current_id);
			
			if (current_id == id) {
				in_target_block = 1;
				found_block = 1;
				in_answer = 0;
				in_analysis = 0;
				printf("找到目标ID %d\n", id);
				continue;
			} else if (in_target_block) {
				break; // 目标块结束
			} else {
				in_target_block = 0;
				continue;
			}
		}
		
		// 在目标块内处理
		if (in_target_block) {
			// 检查标记
			if (strstr(line, "[answer]")) {
				in_answer = 1;
				in_analysis = 0;
				setColor(COLOR_GREEN);
				printf("\n  ═══════════════════ 参考答案代码 ═══════════════════\n");
				setColor(COLOR_DEFAULT);
				content_line_count = 0;
				continue;
			}
			else if (strstr(line, "[分析]")) {
				in_analysis = 1;
				in_answer = 0;
				setColor(COLOR_CYAN);
				printf("\n  ═══════════════════ 详细解析 ═══════════════════\n");
				setColor(COLOR_DEFAULT);
				content_line_count = 0;
				continue;
			}
			// 输出内容
			else if (in_answer || in_analysis) {
				printf("  %s\n", line);
				content_line_count++;
			}
		}
	}
	
	fclose(fp_sol);
	
	if (!found_block) {
		setColor(COLOR_YELLOW);
		printf("\n   未找到ID为 %d 的解析内容。\n", id);
		setColor(COLOR_DEFAULT);
	} else if (content_line_count == 0) {
		setColor(COLOR_YELLOW);
		printf("\n   找到ID为 %d 的解析内容，但无有效数据。\n", id);
		printf("\n  请检查:\n");
		printf("  1. 标记格式: [answer] 和 [分析]（支持前后空格）\n");
		printf("  2. 标记和内容是否换行\n");
		printf("  3. 文件必须为ANSI编码\n");
	}
	
	pause();
}
/*功能级菜单*/
void mainFunctions() {
	while (1) {
		cls();
		printTitle();
		setColor(COLOR_CYAN);
		printf("╔════════════════════════════════════════════════════════════╗\n");
		printf("║                        功能菜单                            ║\n");
		printf("╠════════════════════════════════════════════════════════════╣\n");
		printf("║  【1】 ACM 简介                                            ║\n");
		printf("║  【2】 题库管理                                            ║\n");
		printf("║  【3】 竞赛训练                                            ║\n");
		printf("║  【4】 用户管理                                            ║\n");
		printf("║  【5】 退出系统                                            ║\n");
		printf("╚════════════════════════════════════════════════════════════╝\n");
		setColor(COLOR_DEFAULT);
		
		printf("\n  请选择（1-5）: ");
		int sel; scanf("%d", &sel);
		switch (sel) {
		case 1:
			while (1) {
				cls();
				printTitle();
				setColor(COLOR_BLUE);
				printf("╔════════════════════════════════════════════════════════════╗\n");
				printf("║                      ACM 简介                              ║\n");
				printf("╠════════════════════════════════════════════════════════════╣\n");
				printf("║  【1】 赛事介绍                                            ║\n");
				printf("║  【2】 比赛规则                                            ║\n");
				printf("║  【3】 评分细则                                            ║\n");
				printf("║  【4】 赛事构成                                            ║\n");
				printf("║  【5】 历届获奖                                            ║\n");
				printf("║  【0】 返回上级                                            ║\n");
				printf("╚════════════════════════════════════════════════════════════╝\n");
				setColor(COLOR_DEFAULT);
				
				printf("\n  请选择（0-5）: ");
				int s; scanf("%d", &s);
				if (s == 0) break;
				if (s == 1) showEvent();
				if (s == 2) showRule();
				if (s == 3) showScore();
				if (s == 4) showStructure();
				if (s == 5) showWinners();
			}
			break;
		case 2:
			while (1) {
				cls();
				printTitle();
				setColor(COLOR_MAGENTA);
				printf("╔════════════════════════════════════════════════════════════╗\n");
				printf("║                      题库管理                              ║\n");
				printf("╠════════════════════════════════════════════════════════════╣\n");
				printf("║  【1】 显示赛题                                            ║\n");
				printf("║  【2】 查询赛题                                            ║\n");
				printf("║  【3】 添加赛题                                            ║\n");
				printf("║  【4】 删除赛题                                            ║\n");
				printf("║  【0】 返回上级                                            ║\n");
				printf("╚════════════════════════════════════════════════════════════╝\n");
				setColor(COLOR_DEFAULT);
				
				printf("\n  请选择（0-4）: ");
				int s; scanf("%d", &s);
				if (s == 0) break;
				if (s == 1) displayQuestions();
				if (s == 2) searchQuestion();
				if (s == 3) addQuestion();
				if (s == 4) deleteQuestion();
			}
			break;
		case 3:
			while (1) {
				cls();
				printTitle();
				setColor(COLOR_GREEN);
				printf("╔════════════════════════════════════════════════════════════╗\n");
				printf("║                      竞赛训练                              ║\n");
				printf("╠════════════════════════════════════════════════════════════╣\n");
				printf("║  【1】 检测点答题                                          ║\n");
				printf("║  【2】 智能体训练                                          ║\n");
				printf("║  【3】 赛题题解                                            ║\n");
				printf("║  【4】 题目解析                                            ║\n"); 
				printf("║  【0】 返回上级                                            ║\n");
				printf("╚════════════════════════════════════════════════════════════╝\n");
				setColor(COLOR_DEFAULT);
				
				printf("\n  请选择（0-4）: ");
				int s; scanf("%d", &s); while (getchar() != '\n');
				if (s == 0) break;
				if (s == 1) runTestCases();
				if (s == 2) runUserCode();
				if (s == 3) solveQuestion();
				if (s == 4) showQuestionSolution();
			}
			break;
		case 4:
			while (1) {
				cls();
				printTitle();
				setColor(COLOR_YELLOW);
				printf("╔════════════════════════════════════════════════════════════╗\n");
				printf("║                      用户管理                              ║\n");
				printf("╠════════════════════════════════════════════════════════════╣\n");
				printf("║  【1】 修改密码                                            ║\n");
				printf("║  【2】 注销账户                                            ║\n");  // 修改
				printf("║  【3】 申请重置次数                                        ║\n");
				printf("║  【4】 使用密钥重置                                        ║\n");
				printf("║  【5】 管理员登录                                          ║\n");
				printf("║  【0】 返回上级                                            ║\n");
				printf("╚════════════════════════════════════════════════════════════╝\n");
				setColor(COLOR_DEFAULT);
				
				printf("\n  请选择（0-5）: ");
				int s; scanf("%d", &s); while (getchar() != '\n');
				if (s == 0) break;
				if (s == 1) changePassword();
				if (s == 2) { 
					// 注销账户
					if (deleteUser()) return;  // 删除后返回登录界面
				}
				if (s == 3) applyForReset();
				if (s == 4) useResetKey();
				if (s == 5) { if (adminLogin()) adminMenu(); }
			}
			break;
		case 5:
			cls();
			setColor(COLOR_YELLOW);
			printf("\n  ════════════════════════════════════════════════════════\n");
			printf("  正在退出到登录界面，请稍候...\n");
			printf("  ════════════════════════════════════════════════════════\n");
			setColor(COLOR_DEFAULT);
			Sleep(1000); g_login = 0; return;
			default: 
			setColor(COLOR_RED);
			printf("\n 输入有误！\n"); 
			setColor(COLOR_DEFAULT);
			pause();
		}
	}
}

/*初始单菜单*/
void firstMenu() {
	while (1) {
		cls();
		printTitle();
		setColor(COLOR_CYAN);
		printf("╔════════════════════════════════════════════════════════════╗\n");
		printf("║                    ACM 竞赛管理系统                        ║\n");
		printf("╠════════════════════════════════════════════════════════════╣\n");
		printf("║  【1】 登录                                                ║\n");
		printf("║  【2】 注册                                                ║\n");
		printf("║  【3】 退出                                                ║\n");
		printf("╚════════════════════════════════════════════════════════════╝\n");
		setColor(COLOR_DEFAULT);
		
		printf("\n  请选择（1-3）: ");
		int sel; scanf("%d", &sel);
		if (sel == 3) { 
			cls(); 
			setColor(COLOR_GREEN);
			printf("\n  ════════════════════════════════════════════════════════\n");
			printf("        感谢使用ACM竞赛管理系统，祝您天天AC，再见！\n");
			printf("  ════════════════════════════════════════════════════════\n");
			setColor(COLOR_DEFAULT);
			exit(0); 
		}
		if (sel == 1) { if (login())  { g_login = 1; return; } }
		if (sel == 2) { if (registerUser()) { g_login = 1; return; } }
	}
}

/*main*/
int main() {
	while (1) {
		firstMenu();
		if (g_login) {
			mainFunctions();
			if (!g_login) {
				printf("\n  返回登录界面...\n");
				Sleep(1000); continue;
			}
		}
		break;
	}
	return 0;
}
