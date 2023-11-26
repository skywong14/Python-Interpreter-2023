#pragma once
#include "Bigint.h"
#include <iostream>
#include <cstring>

namespace sjtu {
    //for FFT
    static const int MAXFFT = 5000010;
    const long double PI = acos( - 1.0);
    struct complex{
        long double x, y;
    }a[MAXFFT], b[MAXFFT];
    int limit, exponent, r[MAXFFT];
//int2048::int2048() {

        int2048::int2048(){
            flag = 1;
            size = 1;
            num.clear();
            num.push_back(0);
        }
        int2048::int2048(long long num1){
            num.clear();
            size = 0;
            if (num1 < 0) flag = 0; else flag = 1;
            num1 = std::abs(num1);
            if (num1 == 0){
                num.push_back(0);
                size = 1;
            }else{
                while (num1){
                    size++;
                    num.push_back(num1 % base);
                    num1 /= base;
                }
            }
        }
        int2048::int2048(const std::string &str1){
            read(str1);
        }
        int2048::int2048(const int2048 &n1){
            *this = n1; // / QUESTION this or *this
        }
        //============
        // Integer1
        //============
        void int2048::Check(){
            while (size > 1 && num[size - 1] == 0) {
                size--;
                num.erase(num.begin() + size);
            }
            if (size == 1 && num[0] == 0) flag = 1;
        }
        void int2048::read(const std::string &str1){
            num.clear();
            int head = 0;
            flag = 1; size = 0;
            if (str1[0] == '-') { flag = 0; head = 1; }
            int tmp = 0, count = 0;
            for (int i = str1.length() - 1; i >= head; i--){
                tmp += mul[count]*(str1[i] - '0');
                count++;
                if (count == width){
                    size++;
                    num.push_back(tmp);
                    count = 0;
                    tmp = 0;
                }
            }
            if (count > 0) { size++; num.push_back(tmp); }
            Check();
        }
        // 输出储存的大整数，无需换行
        void int2048::print(){
            if (!flag)  printf("-");//QUESTION or puts("-");?
            printf("%lld", num[size - 1]);
            for (int i = size - 2; i >= 0; i--) {
                int tmp = 10;
                for (int j = 1; j < width; j++, tmp *= 10)
                    if (num[i] < tmp){
                        for (int k = 1;k <= width - j;k++) printf("0");
                        break;
                    }
                printf("%lld", num[i]);
            }
        }

        //ignore flag
        int2048 &int2048::add_positive(const int2048 &other){
            int Size = std::max(size, other.size) + 1;
            for (int i = size + 1; i <= Size; i++)
                num.push_back(0);
            size = Size;
            for (int i = 0; i < Size; i++){
                if (i < other.size) num[i] += other.num[i];
                if (num[i] >= base) { num[i] -= base; num[i + 1]++; }
            }
            Check();
            return *this;
        }
        //abs this >= abs other is guaranteed(ignore flag)
        int2048 &int2048::minus_positive(const int2048 &other){
            for (int i = 0; i < size; i++){
                if (i < other.size) num[i] -= other.num[i];
                if (num[i] < 0) { num[i] += base; num[i + 1]--; }
            }
            Check();
            return *this;
        }

        // 加上一个大整数
        int2048 &int2048::add(const int2048 &other){
            int2048 Self(*this);
            if (Self.flag^other.flag){
                if (Self.flag == 1){ //this > 0 other < 0
                    if (Self > (-other)){
                        Self.minus_positive(other);
                    }else{
                        int2048 Num(other);
                        Num.minus_positive(Self);
                        Self = Num;
                    }
                } else {  //this < 0 other > 0
                    if (Self > (-other)){
                        int2048 Num(other);
                        Num.minus_positive(Self);
                        Self = Num;
                    }else{
                        Self.minus_positive(other);
                    }
                }
            }else{
                Self.add_positive(other);
            }
            *this = Self;
            Check();
            return *this;
        }
        // 返回两个大整数之和
        int2048 add(int2048 n1, const int2048 &n2){
            int2048 n3(n1);
            n3.add(n2);
            return n3;
        }

        // 减去一个大整数
        int2048 &int2048::minus(const int2048 &other){
            int2048 Self(*this);
            Self.add((-other));
            *this = Self;
            return *this;
        }
        // 返回两个大整数之差
        int2048 minus(int2048 n1, const int2048 &n2){
            int2048 n3(n1);
            n3.minus(n2);
            return n3;
        }
        //============
        // Integer2
        //============

        int2048 int2048::operator+() const{
            return *this;
        }
        int2048 int2048::operator-() const{
            int2048 Num(*this);
            if ( Num.size>1 || Num.num[0]!=0 ) Num.flag ^= 1;
            return Num;
        }

        int2048 &int2048::operator=(const int2048 &other){
            flag = other.flag;
            size = other.size;
            num.clear();
            for (int i = 0; i < size; i++) num.push_back(other.num[i]);
            return *this;
        }

        int2048 &int2048::operator+=(const int2048 &other){
            return add(other);
        }
        int2048 operator+(int2048 n1, const int2048 &n2){
            int2048 Num(n1);
            Num.add(n2);
            return Num;
        }

        int2048 &int2048::operator-=(const int2048 &other){
            return minus(other);
        }
        int2048 operator-(int2048 n1, const int2048 &n2){
            int2048 Num(n1);
            Num.minus(n2);
            return Num;
        }

        complex complex_add(complex a, complex b){ a.x += b.x;a.y += b.y;return a;}
        complex complex_minus(complex a, complex b){ a.x -= b.x;a.y -= b.y;return a;}
        complex complex_mul(complex a, complex b){ complex c;c.x = a.x*b.x - a.y*b.y;c.y = a.x*b.y + a.y*b.x;return c;}

        void FFT(complex *arr, int I){ //I = 0  -  >  FFT   I = 1  -  >  IFFT
            for(int i = 0; i < limit; i++)
                if(i < r[i]) std::swap(arr[i], arr[r[i]]);
            for (int mid = 1;mid < limit;mid<<=1){
                complex w, w_n;
                if (I == 0){
                    w_n.x = cos(PI / mid);
                    w_n.y = sin(PI / mid);
                } else {
                    w_n.x = cos(PI / mid);
                    w_n.y =  - sin(PI / mid);
                }
                for (int rr = mid*2, j = 0; j < limit; j += rr){
                    w.x = 1; w.y = 0;
                    for (int k = 0;k < mid;k++){
                        complex x1 = arr[j + k], x2 = complex_mul(arr[j + mid + k], w);
                        arr[j + k] = complex_add(x1, x2);arr[j + mid + k] = complex_minus(x1, x2);
                        w = complex_mul(w, w_n);
                    }
                }
            }
            return;
        }
        int2048 &int2048::operator*=(const int2048 &other){

            int2048 Num(*this);

            int ans_flag = 1;
            if (Num.flag != other.flag) ans_flag = 0;

            //*************
            int N1 = Num.size*2 - 1;
            int N2 = other.size*2 - 1;

            for (int i = 0; i <= (N1 + N2 + 2)*2; i++) {
                a[i].x = 0;a[i].y = 0;
                b[i].x = 0;b[i].y = 0;
            }
            for (int i = 0; i <= N1; i += 2) {
                a[i].x = Num.num[i / 2] % 100ll;
                a[i + 1].x = (long long)(Num.num[i / 2] / 100ll);
            }
            for (int i = 0; i <= N2; i += 2) {
                b[i].x = other.num[i / 2] % 100ll;
                b[i + 1].x = (long long)(other.num[i / 2] / 100ll);
            }
            limit = 1;exponent = 0;
            while(limit <= N1 + N2){
                limit<<=1;
                exponent++;
            }
            for (int i = Num.size + 1; i <= Num.size + other.size; i++) Num.num.push_back(0);
            Num.size = Num.size + other.size;
            for(int i = 0; i < limit; i++)
                r[i] = (r[i>>1]>>1)|((i&1)<<(exponent - 1)) ;
            FFT(a, 0);
            FFT(b, 0);
            for(int i = 0; i <= limit; i++)
                a[i] = complex_mul(a[i], b[i]);
            FFT(a, 1);//IFFT
            for (int i = 0; i <= N1 + N2; i += 2)
                Num.num[i / 2] = (long long)(a[i].x / limit + 0.5) + 100ll*(long long)(a[i + 1].x / limit + 0.5);
            for (int i = 0; i <= N1 + N2 + 2; i++){
                a[i].x = 0;a[i].y = 0;
                b[i].x = 0;b[i].y = 0;
            }
            //************
            for (int i = 0; i < Num.size - 1; i++){
                Num.num[i + 1] += Num.num[i] / base;
                Num.num[i] %= base;
            }
            while (Num.num[Num.size - 1] >= base){
                Num.num.push_back(Num.num[size - 1] / base);
                Num.num[Num.size - 1] %= base;
                Num.size++;
            }
            Num.flag = ans_flag;
            *this = Num;
            Check();
            return *this;
        }
        int2048 operator*(int2048 n1, const int2048 &n2){
            int2048 n3(n1);
            n3 *= n2;
            return n3;
        }

        int2048 int2048::plain_mul(const int2048 &Num, long long Mul, int BASE){  //0 <= Mul < base Mul*(base^BASE)
            if (Mul == 0) {
                int2048 Self(0);
                return Self;
            }
            int2048 Self(Num);
            if (Mul > 1){
                for (int i = 0; i <= Self.size - 1; i++)
                    Self.num[i] *= Mul;
                for (int i = 0; i < Self.size - 1; i++){
                    Self.num[i + 1] += Self.num[i] / base;
                    Self.num[i] %= base;
                }
                if (Self.num[Self.size - 1] >= base){
                    Self.num.push_back(Self.num[Self.size - 1] / base);
                    Self.num[Self.size - 1] %= base;
                    Self.size++;
                }
            }
            for (int i = 1; i <= BASE; i++) Self.num.push_back(0);
            for (int i = Self.size - 1; i >= 0; i--) Self.num[i + BASE] = Self.num[i];
            for (int i = BASE - 1; i >= 0; i--) Self.num[i] = 0;
            Self.size += BASE;
            return Self;
        }


        int2048 int2048::ABS(const int2048 &Num){
            int2048 n1(Num);
            if (n1.flag == 0) n1.flag = 1;
            return n1;
        }
        int2048 int2048::get_head(const int2048 &Num, int pos){
            int2048 n1(0);
            n1.flag = Num.flag;
            n1.num[0] = Num.num[pos];
            for (int i = pos + 1; i < Num.size; i++){
                n1.size++;
                n1.num.push_back(Num.num[i]);
            }
            return n1;
        }
        int2048 &int2048::set_head(const int2048 &other, int pos){
            for (int i = pos; i < pos + other.size; i++)
                num[i] = other.num[i - pos];
            size = pos + other.size;
            while (num.size() > size) num.pop_back();
            Check();
            return *this;
        }
        int2048 int2048::division(const int2048 &n1, const int2048 &n2){
            //normalize
            int2048 A(ABS(n1)), B(ABS(n2));
            int2048 spj(1);
            if (B == spj) return A;
            //special judge
            int2048 C(0);
            A.flag = 1;B.flag = 1;
            if (A < B) return C;
            long long Mul = (base) / (B.num[B.size - 1] + 1);
            /*
            if (B.size > 1)
                Mul = (base*base) / ((B.num[B.size - 1]*base + B.num[B.size - 2]) + 1);
            else
                Mul = (base) / (B.num[B.size - 1] + 1);
            */
            A = plain_mul(A, Mul, 0);
            B = plain_mul(B, Mul, 0);

            int a = A.size - 1, b = B.size - 1;
            long long aa, bb = B.num[b], q_hat = 0;
            std::vector<long long> q_real;
            q_real.clear();
            int2048 mul_B = plain_mul(B, 1, a - 1 - b), A_head;
            for (int i = a; i > b; i--){
                if (A.num.size() >= i + 1)
                    aa = A.num[i]*base + A.num[i - 1]; //ATTENTION: A.num[i] exist?
                else
                    aa = A.num[i - 1];
                q_hat = aa / bb;
                A_head = get_head(A, i - 1 - b);
                A_head -= plain_mul(B, q_hat, 0);
                while (A_head.flag == 0){
                    A_head += B;
                    q_hat--;
                }
                A.set_head(A_head, i - 1 - b);

                mul_B.num.erase(mul_B.num.begin());
                mul_B.size--;

                q_real.push_back(q_hat);
            }
            if (A >= B){
                q_hat = A.num[A.size - 1] / B.num[B.size - 1];
                A -= plain_mul(B, q_hat, 0);
                while (A.flag == 0){
                    A += B;
                    q_hat--;
                }
                q_real.push_back(q_hat);
            }
            int q_size = q_real.size();

            C.num[0] = q_real[q_size - 1];
            for (int i = 1; i <= q_size - 1; i++){
                C.num.push_back(q_real[q_size - i - 1]);
            }
            C.size = q_size;
            return C;
        }


        int2048 &int2048::operator/=(const int2048 &other){
            int2048 Self(*this);
            int ans_flag = 1;
            if (Self.flag == other.flag){
                ans_flag = 1;
                Self = division(Self, other);
            }else{
                ans_flag = 0;
                int2048 q_ans(division(Self, other));
                if (q_ans*other != -Self) {
                    int2048 numt(1);
                    q_ans += numt;
                }
                Self = q_ans;
            }
            Self.flag = ans_flag;
            *this = Self;
            Check();
            return *this;
        }
        int2048 operator/ (int2048 n1, const int2048 &n2){
            int2048 n3(n1);
            n3 /= n2;
            return n3;
        }

        int2048 &int2048::operator %=(const int2048 &other){
            int2048 Self1(*this);
            Self1 /= other;
            int2048 Self2(*this);
            Self2 -= Self1*other;
            *this = Self2;
            Check();
            return *this;
        }
        int2048 operator % (int2048 n1, const int2048 &n2){
            int2048 n3(n1);
            n3 %= n2;
            return n3;
        }
        std::ostream &operator<<(std::ostream &out, const int2048 &n1){
            if (!n1.flag)  out<<"-";//QUESTION or puts("-");?
            out<<n1.num[n1.size - 1];
            for (int i = n1.size - 2; i >= 0; i--) {
                int tmp = 10;
                for (int j = 1; j < n1.width; j++, tmp *= 10)
                    if (n1.num[i] < tmp){
                        for (int k = 1;k <= n1.width - j;k++) out<<"0";
                        break;
                    }
                out<<n1.num[i];
            }
            return out;
        }
        bool operator==(const int2048 &n1, const int2048 &n2){
            if (n1.size != n2.size || n1.flag != n2.flag) return false;
            for (int i = 0; i < n1.size; i++)
                if (n1.num[i] != n2.num[i]) return false;
            return true;
        }//QUESTION need hash to accelerate?
        bool operator!=(const int2048 &n1, const int2048 &n2){
            return !(n1 == n2);
        }
        bool operator<(const int2048 &n1, const int2048 &n2){
            if (n1.flag^n2.flag){ //n1*n2 < 0
                return n2.flag;
            }
            int tag = n1.flag^1; //tag  when positive:0 when negative:1;
            if (n1.size != n2.size){
                return (n1.size < n2.size)^tag;
            }
            for (int i = n1.size - 1; i >= 0; i--)
                if (n1.num[i] != n2.num[i]){
                    return (n1.num[i] < n2.num[i])^tag;
                }
            return false;//n1 == n2
        }
        bool operator>(const int2048 &n1, const int2048 &n2){
            return (n2 < n1);
        }
        bool operator<=(const int2048 &n1, const int2048 &n2){
            return !(n1 > n2);
        }
        bool operator>=(const int2048 &n1, const int2048 &n2){
            return !(n1 < n2);
        }
        double int2048::int2048_to_Double(){
            int2048 x(*this);
            double ans=0;
            for (int i = static_cast<int>(x.num.size())-1; i; i--){
                ans = ans * base + (double)x.num[i];
            }
            if (!x.flag) return -ans;
            return ans;
        }
//}

} // namespace sjtu
