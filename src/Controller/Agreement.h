#ifndef AGREEMENT_H
#define AGREEMENT_H

class Agreement {
   public:
    static bool InitDatabase();
    static bool HasUserAgreed();
    static bool SetUserAgreed();
    static bool ClearUserAgreed();
    static void ShowAgreement();

   private:
    static const char* DATABASE_PATH;
};

#endif