#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
using namespace std;
typedef unsigned int ui;
typedef long long ll;
int bin[35] = {1,2,4,8,16,32,64,128,
    256,512,1024,2048,4096,8192,16384,32768,
    65536,131072,262144,524288,1048576,2097152,4194304,8388608,
    16777216,33554432,67108864,134217728,268435456,536870912,1073741824,-2147483648};
int all_one[35] = {0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535,131071,262143,524287,1048575,2097151,4194303,8388607,16777215,33554431,67108863,134217727,268435455,536870911,1073741823,2147483647,-1};
void sign_extend(int &imm, int w) 
{ 
    if (w == 0) return;
    if (imm & bin[w - 1]) 
        imm |= (all_one[32] ^ all_one[w]); 
}

class Memory_Manage{
public:
    char mem[6000005];
    Memory_Manage()
    {
        char s[15]; int tot;
        while (~scanf("%s", s))
        {
            if (s[0] == '@') 
            {
                tot = 0;
                for (int i = 1; i <= 8; i++)
                {
                    tot = tot * 16 + 
                        ((s[i] >= '0' && s[i] <= '9') ? s[i] - '0' : s[i] - 'A' + 10);
                }
            }
            else
            {
                int num0, num1;
                if (s[0] >= '0' && s[0] <= '9') num0 = s[0] - '0';
                    else num0 = s[0] + 10 - 'A';
                if (s[1] >= '0' && s[1] <= '9') num1 = s[1] - '0';
                    else num1 = s[1] + 10 - 'A';
                mem[tot++] = char(num0 * 16 + num1);
            }
        }
    }
    int load(int pos, int k) 
    {
        int ret = 0;
        for (int i = k / 8 - 1; i >= 0; i--)
            ret = ((ret << 8) | ((ui)mem[pos + i] & all_one[8]));
        return ret;
    }
    void store(int pos, int data, int k)
    {
        for (int i = 0; i < k / 8; i++)
            mem[pos + i] = char((data >> (i * 8)) & all_one[8]);
    }
}o;
class Instruction{
    /*
    * R:   0 ADD, 1 SLL, 2 SLT, 3 SLTU, 4 XOR,
    *      5 SRL, 6 OR, 7 AND, 8 SUB, 9 SRA    (51)
    * 
    * I:   0 ADDI, 1 SLLI, 2 SLTI, 3 SLTIU, 4 XORI, 
    *      5 SRLI, 6 ORI, 7 ANDI, 8 SRAI,      (19)
    *      9 JALR     (103)
    *      10 LB, 11 LH, 12 LW, 14 LBU, 15 LHU   (3)
    * 
    * S:   0 SB, 1 SH, 2 SW    (35)
    * 
    * U:   0 LUI     (55)
    *      1 AUIPC   (23)
    * 
    * B:   0 BEQ, 1 BNE, 4 BLT, 5 BGE, 6 BLTU, 7 BGEU (99)
    * 
    * J:   JAL   (111)
    */
public :
    char type; bool skip;
    int id, imm, rd, rs1, rs2, Rd, Rs1, Rs2, Pc, Pd;
    Instruction() : type('N') {}
    Instruction(int inst) : rd(0), rs1(-1), rs2(-1), imm(0), Pd(0)
    {
        int _id = inst & all_one[7]; 
        if (inst == 267388179) { type = 'D'; return; }
        switch (_id)
        {
            case 51 : 
                type = 'R';
                if ((inst & bin[30]) == 0) 
                    id = ((inst >> 12) & all_one[3]);
                else if (((inst >> 12) & all_one[3]) == 0)
                    id = 8;
                else id = 9;
                rd = ((inst >> 7) & all_one[5]);
                rs1 = ((inst >> 15) & all_one[5]);
                rs2 = ((inst >> 20) & all_one[5]);
                break;
            case 35 :
                type = 'S';
                id = ((inst >> 12) & all_one[3]);
                imm = ((inst >> 7) & all_one[5]);
                imm |= (((inst >> 25) & all_one[7]) << 5);
                sign_extend(imm, 12);
                rs1 = ((inst >> 15) & all_one[5]);
                rs2 = ((inst >> 20) & all_one[5]);
                break;
            case 99 :
                type = 'B';
                id = ((inst >> 12) & all_one[3]);
                imm = ((inst >> 8) & all_one[4]);
                imm |= (((inst >> 25) & all_one[6]) << 4);
                imm |= (((inst >> 7) & 1) << 10);
                imm |= (((inst >> 31) & 1) << 11);
                imm <<= 1;
                sign_extend(imm, 13);
                rs1 = ((inst >> 15) & all_one[5]);
                rs2 = ((inst >> 20) & all_one[5]);
                break;
            case 111 :
                type = 'J';
                imm = (inst >> 31);
                imm = imm << 8 | ((inst >> 12) & all_one[8]);
                imm = imm << 1 | ((inst >> 20) & 1);
                imm = imm << 10 | ((inst >> 21) & all_one[10]);
                imm <<= 1;
                sign_extend(imm, 21);
                rd = ((inst >> 7) & all_one[5]);
                break;
            case 55 :
                type = 'U'; id = 0;
                imm = ((inst >> 12) << 12);
                rd = ((inst >> 7) & all_one[5]);
                break;
            case 23 :
                type = 'U'; id = 1;
                imm = ((inst >> 12) << 12);
                rd = ((inst >> 7) & all_one[5]);
                break;
            case 19 :
                type = 'I';
                id = ((inst >> 12) & all_one[3]);
                rd = ((inst >> 7) & all_one[5]);
                rs1 = ((inst >> 15) & all_one[5]);
                if (id == 1 || id == 5)
                    imm =  ((inst >> 20) & all_one[5]);
                else
                    imm = (inst >> 20);
                if (id == 5 && (inst & bin[30])) id = 8;
                break;
            case 103 :
                type = 'I'; id = 9;
                rd = ((inst >> 7) & all_one[5]);
                rs1 = ((inst >> 15) & all_one[5]);
                imm = (inst >> 20);
                break;
            case 3 :
                type = 'I'; id = 10 + ((inst >> 12) & all_one[3]);
                rd = ((inst >> 7) & all_one[5]);
                rs1 = ((inst >> 15) & all_one[5]);
                imm = (inst >> 20);
                break;
        }
    }
};

class RISCV_Simulator{
public:
    int reg[32], reg_wor[32], pc, reg_IF_ID, MEM_cyc, reg_Pc;
    int cnt[128], suc, tot;
    Instruction reg_ID_EX, reg_EX_MEM, reg_MEM_WB;
    bool ID_wor, EX_wor, MEM_wor, WB_wor;
    
    RISCV_Simulator() : pc(0), reg_IF_ID(0), MEM_cyc(0), suc(0), tot(0)
    {
        for (int i = 0; i < 32; i++) reg[i] = reg_wor[i] = 0;
        for (int i = 0; i < 128; i++) cnt[i] = 0;
        ID_wor = EX_wor = MEM_wor = WB_wor = false;
    }
    void IF() 
    {
        if (ID_wor) return;
        int ret = 0; 
        for (int i = 3; i >= 0; i--)
            ret = ((ret << 8) | ((ui)o.mem[pc + i] & all_one[8]));
        reg_IF_ID = ret; reg_Pc = pc;
        pc += 4;
        ID_wor = true;
    }
    void ID()
    {
        if (!ID_wor || EX_wor || reg_IF_ID == 267388179) return;
        Instruction ins = Instruction(reg_IF_ID);
        ins.Pc = reg_Pc;
        if ((~ins.rs1) && reg_wor[ins.rs1]) return;
        if ((~ins.rs2) && reg_wor[ins.rs2]) return;
        ID_wor = false; EX_wor = true;
        if (ins.rd) reg_wor[ins.rd]++;
        ins.Rd = reg[ins.rd];
        if (~ins.rs1) ins.Rs1 = reg[ins.rs1];
        if (~ins.rs2) ins.Rs2 = reg[ins.rs2]; 
        switch (ins.type)
        {
            case 'I':
                if (ins.id == 9) 
                    pc = ((ins.Rs1 + ins.imm) & (~1)); 
                break;
            case 'B' :
                ins.skip = (cnt[ins.Pc & all_one[7]] & 2);
                if (ins.skip) pc = ins.Pc + ins.imm;
                    else pc = ins.Pc + 4; //????
                tot++;
                break;
            case 'J' : 
                pc = ins.Pc + ins.imm; break;
        }
        reg_ID_EX = ins;
    }
    void EX()
    {
        if (!EX_wor || MEM_wor) return;
        Instruction ins = reg_ID_EX;
        switch (ins.type)
        {
            case 'R':
                switch (ins.id)
                {
                    case 0 : ins.Rd = ins.Rs1 + ins.Rs2; break;
                    case 1 : ins.Rd = (ui)ins.Rs1 << (ins.Rs2 & all_one[5]); break;
                    case 2 : ins.Rd = ins.Rs1 < ins.Rs2; break;
                    case 3 : ins.Rd = (ui)ins.Rs1 < (ui)ins.Rs2; break;
                    case 4 : ins.Rd = ins.Rs1 ^ ins.Rs2; break;
                    case 5 : ins.Rd = (ui)ins.Rs1 >> (ins.Rs2 & all_one[5]); break;
                    case 6 : ins.Rd = ins.Rs1 | ins.Rs2; break;
                    case 7 : ins.Rd = ins.Rs1 & ins.Rs2; break;
                    case 8 : ins.Rd = ins.Rs1 - ins.Rs2; break;
                    case 9 : ins.Rd = ins.Rs1 >> (ins.Rs2 & all_one[5]); break;
                }
                break;
            case 'I':
                switch (ins.id)
                {
                    case 0 : ins.Rd = ins.Rs1 + ins.imm; break;
                    case 1 : ins.Rd = (ui)ins.Rs1 << ins.imm; break;
                    case 2 : ins.Rd = ins.Rs1 < ins.imm; break;
                    case 3 : 
                        if (ins.imm == 1) 
                        {
                            ins.Rd = ins.Rs1 == 0;
                            printf("NEQZ!!!!!!!!\n");
                            exit(0);
                        }
                        else ins.Rd = (ui)ins.Rs1 < (ui)ins.imm;
                        break;
                    case 4 : ins.Rd = ins.Rs1 ^ ins.imm; break;
                    case 5 : ins.Rd = (ui)ins.Rs1 >> ins.imm; break;
                    case 6 : ins.Rd = ins.Rs1 | ins.imm; break;
                    case 7 : ins.Rd = ins.Rs1 & ins.imm; break;
                    case 8 : ins.Rd = ins.Rs1 >> ins.imm; break;
                    case 9 :  ins.Rd = ins.Pc + 4; break;
                    default : ins.Pc = ins.Rs1 + ins.imm; break;
                }
                break;
            case 'S' :
                ins.Pc = ins.Rs1 + ins.imm; break;
            case 'U' :
                if (ins.id == 0) ins.Rd = ins.imm;
                    else ins.Rd = ins.Pc + ins.imm;
                break;
            case 'B' :
                bool skip_to;
                switch (ins.id)
                {
                    case 0 : skip_to = ins.Rs1 == ins.Rs2; break;
                    case 1 : skip_to = ins.Rs1 != ins.Rs2; break;
                    case 4 : skip_to = ins.Rs1 < ins.Rs2; break;
                    case 5 : skip_to = ins.Rs1 >= ins.Rs2; break;
                    case 6 : skip_to = (ui)ins.Rs1 < (ui)ins.Rs2; break;
                    case 7 : skip_to = (ui)ins.Rs1 >= (ui)ins.Rs2; break;
                }
                if (skip_to) 
                {
                    if (ins.skip) suc++;
                    else
                    {
                        pc = ins.Pc + ins.imm; 
                        ID_wor = false;
                    }
                    cnt[ins.Pc & all_one[7]] = min(cnt[ins.Pc & all_one[7]] + 1, 3);
                }
                else
                {
                    if (!ins.skip) suc++;
                    else
                    {
                        pc = ins.Pc + 4; 
                        ID_wor = false;
                    }
                    cnt[ins.Pc & all_one[7]] = max(cnt[ins.Pc & all_one[7]] - 1, 0);
                }
                break;
            case 'J' : 
                ins.Rd = ins.Pc + 4; break;
        }
        EX_wor = false; MEM_wor = true; MEM_cyc = 0;
        reg_EX_MEM = ins;
    }
    void MEM()
    {
        if (!MEM_wor || WB_wor) return;
        Instruction ins = reg_EX_MEM;
        if (ins.type == 'S' || (ins.type == 'I' && ins.id > 9))
        {
            if (MEM_cyc < 3) MEM_cyc++;
            else
            {
                if (ins.type == 'S')
                {
                    switch (ins.id)
                    {
                        case 0 : o.store(ins.Pc, ins.Rs2, 8); break;
                        case 1 : o.store(ins.Pc, ins.Rs2, 16); break;
                        case 2 : o.store(ins.Pc, ins.Rs2, 32); break;
                    }
                }
                else if (ins.type == 'I')
                {
                    switch (ins.id)
                    {
                        case 10 : ins.Rd = o.load(ins.Pc, 8); sign_extend(ins.Rd, 8); break;
                        case 11 : ins.Rd = o.load(ins.Pc, 16); sign_extend(ins.Rd, 16); break;
                        case 12 : ins.Rd = o.load(ins.Pc, 32); break;
                        case 14 : ins.Rd = o.load(ins.Pc, 8); ins.Rd &= all_one[8]; break;
                        case 15 : ins.Rd = o.load(ins.Pc, 16); ins.Rd &= all_one[16]; break;
                    }
                }
                MEM_wor = false; WB_wor = true;
            }
        }
        else
        {           
            MEM_wor = false; WB_wor = true;
        }
        reg_MEM_WB = ins;
    }
    void WB()
    {
        if (!WB_wor) return;
        Instruction ins = reg_MEM_WB;
        if (ins.rd != 0) 
        {
            reg[ins.rd] = ins.Rd;
            reg_wor[ins.rd]--;
        }
        WB_wor = false;
    }
    bool fin() { return reg_IF_ID == 267388179 && ID_wor && !EX_wor && !MEM_wor && !WB_wor; }
}simul;
int main()
{
    int tot = 0;
    while (!simul.fin())
    {
        tot++;
        simul.WB();
        simul.MEM();
        simul.EX(); 
        simul.ID();
        simul.IF();
    }
    printf("%d\n", (simul.reg[10] & 255u));
    return 0;
}