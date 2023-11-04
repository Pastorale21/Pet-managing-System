#include<iostream>
#include<string>
#include<vector>
#include <sstream> 
#include <fstream>
#include<algorithm>
using namespace std;


struct Master//编号、主人姓名、联系方式、寄养宠物的数量
{
    string bianhao_m;
    string name_m;
    long long phone;
    int number;
    int totalSpending;  
    Master* next;
};
struct Pet//编号、名字、品种、主人编号，寄养天数，每天价格
{
    string bianhao_p;
    string name_p;
    string species;
    string bianhao_m;
    int day;
    int everydayprice;
    Pet* next;
    Master* owner;  // 指向宠物的主人信息节点
};
Master* owners = nullptr;  // 主人信息使用链表存储——头指针
Pet* pets = nullptr;      // 宠物信息使用链表存储

void addMasterNode(string id, string name, long long phone, int number) {
    Master* newMaster = new Master();
    newMaster->bianhao_m = id;
    newMaster->name_m = name;
    newMaster->phone = phone;
    newMaster->number = number;
    newMaster->next = nullptr;
    if(owners == nullptr)
        owners = newMaster;
    else
    {
        Master* currentMaster = owners;
        while (currentMaster->next != nullptr) {
            currentMaster = currentMaster->next;  // 找到链表的末尾
        }
        currentMaster->next = newMaster;  // 将新节点插入到末尾

    }
}
void addPetNode(string id, string name, string species, string ownerId, int days, int price) {
    Pet* newPet = new Pet();
    newPet->bianhao_p = id;
    newPet->name_p = name;
    newPet->species = species;
    newPet->bianhao_m = ownerId;
    newPet->day = days;
    newPet->everydayprice = price;
    newPet->next = nullptr;  // 新节点的下一个节点为nullptr，表示链表的末尾

    if (pets == nullptr) {
        pets = newPet;  // 如果链表为空，直接将新节点设为链表的头部
    } else {
        Pet* currentPet = pets;
        while (currentPet->next != nullptr) {
            currentPet = currentPet->next;  // 找到链表的末尾
        }
        currentPet->next = newPet;  // 将新节点插入到末尾
    }
}

void readPetData() {
    ifstream file("pet_data.txt");  // 假设宠物信息文件名为 pet_data.txt

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            // 使用空格分割每一行的信息
            // 假设文件格式为: 宠物编号 宠物名字 宠物品种 主人编号 寄养天数 每天价格
            int  day, price;
            string id;
            string ownerId;
            string name, species;
            ss >> id >> name >> species >> ownerId >> day >> price;
            addPetNode(id, name, species, ownerId, day, price);
        }

        file.close();
    } else {
        cout << "无法打开宠物信息文件。" << endl;
    }
}

// 读取主人信息文件
void readOwnerData() {
    ifstream file("owner_data.txt");  // 假设宠物主人信息文件名为 owner_data.txt

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            // 使用空格分割每一行的信息
            // 假设文件格式为: 主人编号 主人名字 联系方式 寄养宠物数量
            string id;
            string name;
            long long phone;
            int number;
            ss >> id >> name >> phone >> number;
            addMasterNode(id, name, phone, number);
        }

        file.close();
    } else {
        cout << "无法打开宠物主人信息文件。" << endl << endl;
    }
}
void queryPetAndOwnerInfo() {
    string petName;
    cout << "请输入要查询的宠物名字或编号: ";
    cin >> petName;

    // 在 pets 链表中查找符合条件的宠物
    Pet* currentPet = pets;
    while (currentPet != nullptr) {
        if (currentPet->name_p == petName||currentPet->bianhao_p == petName) {
            // 输出宠物信息
            cout << "宠物信息：" << endl;
            cout << "编号: " << currentPet->bianhao_p << endl;
            cout << "名字: " << currentPet->name_p << endl;
            cout << "品种: " << currentPet->species << endl;
            cout << "寄养天数: " << currentPet->day << endl;
            cout << "每天价格: " << currentPet->everydayprice << endl;

            // 查找该宠物的主人信息
            Master* currentOwner = owners;
            while (currentOwner != nullptr) {
                if (currentOwner->bianhao_m == currentPet->bianhao_m) {
                    // 输出主人信息
                    cout << "主人信息：" << endl;
                    cout << "编号: " << currentOwner->bianhao_m << endl;
                    cout << "名字: " << currentOwner->name_m << endl;
                    cout << "联系方式: " << currentOwner->phone << endl;
                    cout << "寄养宠物数量: " << currentOwner->number << endl;
                    break; // 找到符合条件的主人，退出循环
                }
                currentOwner = currentOwner->next;
            }

            return; // 找到了符合条件的宠物，结束函数
        }
        currentPet = currentPet->next;
    }

    // 如果循环结束仍然没有找到符合条件的宠物
    cout << "未找到符合条件的宠物信息。" << endl << endl;
}

void showMenu() {
    // 显示菜单
    cout << "========== 宠物寄养管理系统 ==========" << endl;
    cout << "1. 查询宠物信息与主人信息" << endl;
    cout << "2. 删除宠物信息" << endl;
    cout << "3. 修改宠物信息" << endl;
    cout << "4. 增加宠物信息与主人信息" << endl;
    cout << "5. 花费排序" << endl;
    cout << "6. 保存宠物信息与主人信息" << endl;
    cout << "7. 退出系统" << endl;
    cout << "=====================================" << endl;
}
// 删除宠物信息
void deletePetInfo(const string& petName) {
    Pet* prevPet = nullptr;
    Pet* currentPet = pets;

    // 处理删除第一个节点的情况
    if (currentPet != nullptr && (currentPet->name_p == petName||currentPet->bianhao_p == petName)) {
        pets = currentPet->next;
        delete currentPet;
        cout << "成功删除宠物信息：" << currentPet->name_p << endl << endl;
        return;
    }

    // 处理删除非第一个节点的情况
    while (currentPet != nullptr) {
        if (currentPet->name_p == petName||currentPet->bianhao_p == petName) {
            // 找到符合条件的宠物
            string output = currentPet->name_p; 
            // 如果当前宠物的主人只有这一个宠物，则删除该主人信息
            string ownerId = currentPet->bianhao_m;
            Master* currentOwner = owners;
            Master* prevOwner = nullptr;
            while (currentOwner != nullptr) {
                if (currentOwner->bianhao_m == ownerId) {
                    if (currentOwner->number == 1) {
                        if (prevOwner == nullptr) {
                            owners = currentOwner->next;
                        } else {
                            prevOwner->next = currentOwner->next;
                        }
                        delete currentOwner;
                    } else {
                        currentOwner->number--;
                    }

                    break;
                }
                prevOwner = currentOwner;
                currentOwner = currentOwner->next;
            }

            // 删除宠物信息
            if (prevPet != nullptr) {
                prevPet->next = currentPet->next;
            }
            delete currentPet;
            cout << "成功删除宠物信息：" << output << endl << endl;
            return;
        }

        prevPet = currentPet;
        currentPet = currentPet->next;
    }

    // 如果循环结束仍然没有找到符合条件的宠物
    cout << "未找到符合条件的宠物信息" << endl << endl;
}

// 修改宠物信息
void modifyPetInfo(string petId) {
    Pet* currentPet = pets;

    while (currentPet != nullptr) {
        if (currentPet->name_p == petId||currentPet->bianhao_p == petId) {
            // 找到符合条件的宠物

            int newDay, newPrice;
            cout << "请输入新的寄养天数: ";
            cin >> newDay;
            cout << "请输入新的每天价格: ";
            cin >> newPrice;

            // 修改宠物信息
            currentPet->day = newDay;
            currentPet->everydayprice = newPrice;

            cout << "成功修改宠物信息：" << petId << endl << endl;
            return;
        }

        currentPet = currentPet->next;
    }

    // 如果循环结束仍然没有找到符合条件的宠物
    cout << "未找到宠物编号或名称为 " << petId << " 的宠物信息。" << endl << endl;
}


// 增加宠物信息与主人信息
void addPetAndOwnerInfo() {

    int  ownerId, days, price;
    string petId;
    string petName, petSpecies, ownerName;
    string ownerBianhao;
    long long ownerPhone;

    // 输入宠物信息
    cout << "请输入新宠物的编号: ";
    cin >> petId;
    cout << "请输入新宠物的名字: ";
    cin >> petName;
    cout << "请输入新宠物的品种: ";
    cin >> petSpecies;
    cout << "请输入新宠物的寄养天数: ";
    cin >> days;
    cout << "请输入新宠物的每天价格: ";
    cin >> price;

    // 输入主人信息
    cout << "请输入新主人的编号: ";
    cin >> ownerBianhao;
    cout << "请输入新主人的名字: ";
    cin >> ownerName;
    cout << "请输入新主人的联系方式: ";
    cin >> ownerPhone;

   
    Master* currentOwner = owners;
    while (currentOwner != nullptr) {
        if (currentOwner->bianhao_m == ownerBianhao) {
            addPetNode(petId, petName, petSpecies, ownerBianhao, days, price);
            cout << "成功添加新宠物和主人信息。" << endl; 
            return;
        }
        
        currentOwner = currentOwner->next;
    }

    // 添加新主人信息
    addMasterNode(ownerBianhao, ownerName, ownerPhone, 1);

    // 添加新宠物信息
    addPetNode(petId, petName, petSpecies, ownerBianhao, days, price);

    cout << "成功添加新宠物和主人信息。" << endl;
}
void addPetAndOwnerInfo(string ownerBianhao) {
    int days, price;
    string petId, petName, petSpecies;
    Master* currentOwner = owners;
    while (currentOwner != nullptr){
        if (currentOwner->bianhao_m == ownerBianhao||currentOwner->name_m == ownerBianhao){
            break;
        }
        else{
            cout << "未找到指定编号的主人，无法添加宠物信息。" << endl;
            return;
        }

    } 

    // 输入宠物信息
    cout << "请输入新宠物的编号: ";
    cin >> petId;
    cout << "请输入新宠物的名字: ";
    cin >> petName;
    cout << "请输入新宠物的品种: ";
    cin >> petSpecies;
    cout << "请输入新宠物的寄养天数: ";
    cin >> days;
    cout << "请输入新宠物的每天价格: ";
    cin >> price;

    // 在已有的主人下添加宠物信息
    
    while (currentOwner != nullptr) {
        if (currentOwner->bianhao_m == ownerBianhao||currentOwner->name_m == ownerBianhao) {    
            // 添加新宠物信息
            addPetNode(petId, petName, petSpecies, ownerBianhao, days, price);
            currentOwner->number++; // 增加主人的宠物数量
            cout << "成功添加新宠物信息到已有主人。" << endl;
            return;
        }
        currentOwner = currentOwner->next;
    }

    
    
}


// 按花费排序
void sortBySpending() {
    Master* currentOwner = owners;
    vector<Master*> ownerPointers;  // 用于存储主人节点的指针

    // 将主人节点的指针放入向量中
    while (currentOwner != nullptr) {
        ownerPointers.push_back(currentOwner);
        currentOwner = currentOwner->next;
    }

    // 使用lambda表达式进行排序，按照总花费从高到低排序
    sort(ownerPointers.begin(), ownerPointers.end(), [](Master* a, Master* b) {
        return a->totalSpending > b->totalSpending;
    });

    // 输出排序后的主人信息
    cout << "按照花费排序后的主人信息：" << endl;
    for (const auto& ownerPtr : ownerPointers) {
        cout << "主人编号: " << ownerPtr->bianhao_m << endl;
        cout << "主人姓名: " << ownerPtr->name_m << endl;
        cout << "联系方式: " << ownerPtr->phone << endl;
        cout << "寄养宠物数量: " << ownerPtr->number << endl;
        cout << "总花费: " << ownerPtr->totalSpending << endl;
        cout << "--------------------" << endl;
    }
}



// 保存宠物信息与主人信息到文件
void saveDataToFile() {
    // 保存宠物信息到文件
    ofstream petFile("pet_data.txt");
    if (petFile.is_open()) {
        Pet* currentPet = pets;
        while (currentPet != nullptr) {
            petFile << currentPet->bianhao_p << " "
                    << currentPet->name_p << " "
                    << currentPet->species << " "
                    << currentPet->bianhao_m << " "
                    << currentPet->day << " "
                    << currentPet->everydayprice << endl;
            currentPet = currentPet->next;
        }
        
        petFile.close();
        cout << "宠物信息已保存到 pet_data.txt 文件。" << endl;
    } else {
        cout << "无法打开 pet_data.txt 文件，宠物信息保存失败。" << endl;
    }

    // 保存主人信息到文件
    ofstream ownerFile("owner_data.txt");
    if (ownerFile.is_open()) {
        Master* currentOwner = owners;
        while (currentOwner != nullptr) {
            ownerFile << currentOwner->bianhao_m << " "
                      << currentOwner->name_m << " "
                      << currentOwner->phone << " "
                      << currentOwner->number << endl;
            currentOwner = currentOwner->next;
        }
        ownerFile.close();
        cout << "主人信息已保存到 owner_data.txt 文件。" << endl;
    } else {
        cout << "无法打开 owner_data.txt 文件，主人信息保存失败。" << endl;
    }
}

void calculateTotalSpending()
{
    Master* currentOwner = owners;
    Pet* currentPet = pets;
    while (currentOwner != nullptr)
    {
        while (currentPet != nullptr)
        {
            if (currentPet->bianhao_m == currentOwner->bianhao_m)
            {
                currentOwner->totalSpending += currentPet->day * currentPet->everydayprice;
            }
            currentPet = currentPet->next;
        }
        currentPet = pets;
        currentOwner = currentOwner->next;
    }
    
}
int main()
{
    readOwnerData();
    readPetData();
    calculateTotalSpending();
    while (true) 
    {
        showMenu();
        int choice;
        
        cout << "请输入选项: ";
        cin >> choice;
        cout << endl;
        
        if (choice == 7)
            break;
        
    

        switch (choice)
        {
            case 1:
            {
                queryPetAndOwnerInfo();
                break;
            }
            case 2:
            {
                cout<<"请输入需要删除的宠物名称或编号"<<endl;
                string petName;
                cin>>petName;
                deletePetInfo(petName);
                break;
            }
            case 3:
            {
                string petid;
                cout<<"请输入需要修改的宠物编号或名称"<<endl;
                cin>>petid;
                modifyPetInfo(petid);
                break;
            }
            case 4:
            {
                int flag;
                string bianhao;
                
                cout << "在已有主人下添加宠物键入0       新建主人文档以存储宠物信息键入1:";
                cout << endl;
                cin >> flag;
                if(flag)
                    addPetAndOwnerInfo(); 
                else
                {
                    cout<<"请键入主人id"<<endl;
                    cin>>bianhao;
                    addPetAndOwnerInfo(bianhao); 
                } 
                
                break;
            }
            case 5:
            {
                sortBySpending();
                break;
            }
            case 6:
            {
                saveDataToFile();
                
                break;
            }
            default :
            {
                cout<<"无效输入,请重新输入"<<endl;
                break;
            }
        }
    }
    return 0;
    
}