
# Trình biên dịch
CXX = g++

# Đường dẫn Raylib trên máy của bạn
RAYLIB_PATH = /home/van/Applications/Linux/raylib/src

# Cấu trúc thư mục của bạn
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Tên file chạy (sẽ nằm trong thư mục build)
TARGET = $(BUILD_DIR)/game_app

# Tự động tìm TẤT CẢ các file .cpp trong thư mục source
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Thêm -MMD -MP để tự động tạo file dependency (.d) cho header
CXXFLAGS = -I$(INC_DIR) -I$(RAYLIB_PATH) -Wall -std=c++17 -MMD -MP

# Thêm flag ngoài 
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(FLAGS) -c $< -o $@

# Cờ liên kết thư viện
LDFLAGS = -L$(RAYLIB_PATH) -Wl,-rpath,$(RAYLIB_PATH) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# --- CÁC LỆNH (TARGETS) ---
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Tự động nạp các file dependency vào Makefile
-include $(OBJS:.o=.d)

run: all
	./$(TARGET) $(ARGS)

clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all run clean

