# Trình biên dịch
CXX = g++

# Đường dẫn Raylib trên máy của bạn
RAYLIB_PATH = $(CURDIR)/third_party/raylib/src

# ⬅️ THÊM: Đường dẫn Quill (vendored trong dự án)
QUILL_PATH  = $(CURDIR)/third_party/quill

# Cấu trúc thư mục
SRC_DIR = src
BUILD_DIR = build

# Tên file chạy (sẽ nằm trong thư mục build)
TARGET = $(BUILD_DIR)/game_app

# 🚀 TỰ ĐỘNG TÌM ĐỆ QUY TẤT CẢ FILE .cpp TRONG CÁC THƯ MỤC CON
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
# Chuyển đổi đường dẫn file .cpp thành file .o tương ứng trong build/
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# 🛠️ CỜ BIÊN DỊCH
# ⬅️ THÊM: -pthread (bắt buộc cho Quill) và -isystem $(QUILL_PATH)/include
#    -isystem thay vì -I để tắt warning từ header Quill (không cần thiết nhưng sạch log)
CXXFLAGS = -I. -I$(RAYLIB_PATH) \
           -isystem $(QUILL_PATH)/include \
           -Wall -std=c++20 -MMD -MP -pthread \
           -O2 -DNDEBUG

# Cờ liên kết thư viện
# ⬅️ THÊM: -pthread ở cuối (link flag) — -lpthread đã có nhưng -pthread tốt hơn
LDFLAGS = -L$(RAYLIB_PATH) -Wl,-rpath,$(RAYLIB_PATH) \
          -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -pthread

# --- CÁC LỆNH (TARGETS) ---
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Biên dịch file .cpp thành .o và tự động tạo thư mục đích tương ứng
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Tự động nạp các file dependency (.d) để theo dõi thay đổi của file header
-include $(OBJS:.o=.d)

# Lệnh chạy game có hỗ trợ truyền tham số qua biến ARGS="..."
run: all
	./$(TARGET) $(ARGS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
