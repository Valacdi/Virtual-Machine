#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	unsigned char regs[16]; // the char type is actually an 8-bit integer - exactly the size of a single byte
	char prog_mem[256];
	ifstream decryptor("decryptor.bin", ios::binary);
	ifstream encryptor("q1_encr.txt", ios::binary);
	ofstream fr("rez.txt");
	for (int i = 0; i < 256; i++)
	{
        decryptor >> noskipws >> prog_mem[i];
        if (decryptor.eof() == true)
            break;
	}
	decryptor.close();

	//for (int i = 0; i < 256; i+=2)
		//cout << i << ".  " << (int)prog_mem[i] << " " << (int)prog_mem[i + 1] << endl;

	bool veikimas = true, flag = false;
	int i = 0;
	while (veikimas)
	{
		char instrukcija, parametras;
		int Rx, Ry, konstanta;
		bool kitas = true;

		instrukcija = prog_mem[i];
		parametras = prog_mem[i + 1];
		Rx = parametras & 0x0F;
		Ry = (parametras & 0xF0) >> 4;
		konstanta = (int)parametras;

		//cout << "i= " << i << "       " << (int)instrukcija << " " << (int)parametras << " " << Ry << " " << Rx << " " << konstanta << endl << endl;

		switch (instrukcija)
		{
		case 0x01:
			regs[Rx] = regs[Rx] + 1;
			break;

		case 0x02:
			regs[Rx] = regs[Rx] - 1;
			break;

		case 0x03:
			regs[Rx] = regs[Ry];
			break;

		case 0x04:
			regs[0] = (unsigned char)parametras;
			break;

		case 0x05:
			regs[Rx] = (regs[Rx] << 1);
			break;

		case 0x06:
			regs[Rx] = (regs[Rx] >> 1);
			break;

		case 0x07:
			i = i + konstanta;
            kitas = false;
			break;

		case 0x08://
			if (!flag)
            {
                i = i + konstanta;
                kitas = false;
            }
			break;

		case 0x09://
			if (flag)
            {
                i = i + konstanta;
                kitas = false;
            }
			break;

		case 0x0a:
			if (flag)
            {
                i = i + konstanta;
                kitas = false;
            }
			break;

		case 0x0b:
			cout << "\n\nBaigta.\n";
			veikimas = false;
			break;

		case 0x0c:
			regs[Rx] = regs[Rx] + regs[Ry];
			break;

		case 0x0d:
			regs[Rx] = regs[Rx] - regs[Ry];
			break;

		case 0x0e:
			regs[Rx] = (regs[Rx] | regs[Ry]) & (~(regs[Rx] & regs[Ry]));
            break;

		case 0x0f:
			regs[Rx] = (regs[Rx] | regs[Ry]);
			break;

		case 0x10:
			char baitas;
			if (encryptor.get(baitas))
            {
				flag = false;
				regs[Rx] = (unsigned char)baitas;
			}
			else if (encryptor.eof())
                flag = true;
			break;

        case 0x11:
			cout << regs[Rx];
			fr << regs[Rx];
			break;

		default:
            break;
		}

        if (kitas) ///rodo i sekancia komanda
			i += 2;
	}
	encryptor.close();
	fr.close();

	return 0;
}
