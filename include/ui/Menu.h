// Menu.h
// Module M2: Khai bao cac menu lua chon cua he thong
#ifndef CTDL_26_27_HCMUE_MENU_H
#define CTDL_26_27_HCMUE_MENU_H

namespace Menu {

    // 1. Menu chinh cua chuong trinh
    // - 1: Hien thi danh sach cac chuyen bay
    // - 2: Dat ve
    // - 3: Quan ly (dang nhap)
    // - 0: Thoat
    int showMain();

    // 2. Menu quan ly (sau khi dang nhap thanh cong)
    // - 1: Xu ly dat ve
    // - 2: Xu ly tra ve
    // - 3: Thong ke
    // - 0: Dang xuat (quay ve menu chinh)
    int showAdmin();

    // 3. Menu thong ke
    // - 1: Danh sach khach cua mot chuyen bay
    // - 2: Danh sach ghe con trong cua mot chuyen bay
    // - 3: Thong ke so luong thuc hien chuyen bay cua mot may bay
    // - 0: Quay lai menu quan ly
    int showStatistics();

}  // namespace Menu

#endif // CTDL_26_27_HCMUE_MENU_H
