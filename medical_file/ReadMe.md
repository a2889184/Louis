基本設定

1.自訂一個資料夾(名稱自訂)，但是要內含兩個子資料夾，且名稱分別為 file, data。

2.將所有 .m 檔都放在 file 資料夾裡，而要處理的檔案都放在data裡(包含資料夾都一起放進去，像是Control, Vibrator這些資料夾等)

執行前處理

1.先把 Current Folder 換到file裡

2.在 Command Window上 輸入 process_data_directory('../data/Control') (重複步驟並將Control改成其哪data資料夾名稱) (此步驟是把最後要輸出圖片和數據的資料夾名稱先存起來)

3.在 Command Window上 輸入 createfolder('../data/Control', './data_directory/Control') (重複步驟並將Control改成其哪data資料夾名稱) (這步是將第2步的資料夾生成出來，所以最後輸出的檔案會放在data/XXXX/output裡，XXXX可為Control, Vibrator等等)

數據處理

A. 一次處理全部檔案
打開ProcessAllData.m，將第三行及第八行的Control改成欲處理的資料夾名稱後，在Command Window上輸入 ProcessAllData 將會一次處理完該資料夾所有的檔案為止。

B. 一次處理一個檔案
打開GetData.m，將第三行及第八行的Control改成欲處理的資料夾名稱後，及第11行和12行改成想要處理的子資料夾及檔案名稱，在Command Window上輸入GetData就會處理完該檔案的數據。

生成檔案

A.圖片部分
1. 如果名稱裡有UP或DOWN(DN)分別表示腳的墊腳過程、放下過程。注意: UP1是第二次墊腳，UP2是第三次墊腳，DN1是第一次放下，DN2是第二次放下。
2. 若標題有smooth則是將整數秒的前後0.5秒全部加起來取平均後的結果
3. 標題中有Accel Y及Y都是指加速規

B.數據部分
每個.mat檔裡都有一個名稱沒有_partial的跟有_partial的cell，有_partial的顧名思義將墊腳、放下這幾個過程分開來看。
沒partial的每一行都有三列資料，第一列是資料名稱，第二列是f50的數值，第三列是f80數值
有partial的每一行也有三列資料，第一列是資料名稱，第二列是一個陣列，裡面存的是第一次上升(或下降)的f50及f80，第三列也是一個陣列，裡面存的是第二次上升(或下降)的f50及f80