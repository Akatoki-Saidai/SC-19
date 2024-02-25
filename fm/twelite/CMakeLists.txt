# ビルドを実行するファイルを追加
add_library(TWELITE STATIC
    ${CMAKE_CURRENT_LIST_DIR}/twelite.cpp
)
# 以下の資料を参考にしました
# https://qiita.com/kikochan/items/732e46e92e7f29c18ce9
# https://qiita.com/shohirose/items/45fb49c6b429e8b204ac

# ライブラリの読み込み
target_link_libraries(TWELITE
        pico_stdlib 
        SC
)