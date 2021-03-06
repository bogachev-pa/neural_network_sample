vpath %.cpp src
vpath %.tcc src
vpath %.h src

OBJDIR = obj
BINDIR = bin
PLOTDIR = plot

CC = g++ -std=c++03 -O3
CFLAGS = $(CC) -Wall

ifeq ($(DEBUG), y)
  CFLAGS += -DNN_DEBUG
else
  CFLAGS += -DNDEBUG
endif

ifeq ($(CLASSIFICATION), y)
  CFLAGS += \
    -DNN_CLASSIFICATION_TASK \
    -DNN_PLOT_COORD \
    -DNN_USE_PERCEPTRON \
    -DNN_ENABLE_POLARIZED \
    -DNN_NUM_LAYERS=1 \
    -DNN_NUM_TEACHING_ITERATIONS=50
else
  CFLAGS += \
    -DNN_TIC_TAC_TOE_TASK \
    -DNN_USE_SIGMOIDAL \
    -DNN_TRAIN_BACK_PROPAGATE \
    -DNN_CHECK_SEPARATE \
    -DNN_NUM_LAYERS=2 \
    -DNN_NUM_TEACHING_ITERATIONS=2000
endif

nn_binary = $(BINDIR)/nn
nn_objects = \
  $(OBJDIR)/main.o \
  $(OBJDIR)/neural_network.o \
  $(OBJDIR)/layer.o \
  $(OBJDIR)/neuron.o \
  $(OBJDIR)/sigmoidal.o \
  $(OBJDIR)/perceptron.o \
  $(OBJDIR)/training_set.o \
  $(OBJDIR)/plot.o \
  $(OBJDIR)/util.o

all: nn

nn: $(nn_objects)
	@mkdir -p $(BINDIR)
	$(CFLAGS) -o $(nn_binary) $(nn_objects)

$(OBJDIR)/main.o: main.cpp
	@mkdir -p $(OBJDIR)
	$(CFLAGS) -c $< -o $@

$(OBJDIR)/neural_network.o: neural_network.cpp
	@mkdir -p $(OBJDIR)
	$(CFLAGS) -c $< -o $@

$(OBJDIR)/layer.o: layer.cpp
	@mkdir -p $(OBJDIR)
	$(CFLAGS) -c $< -o $@

$(OBJDIR)/neuron.o: neuron.cpp
	@mkdir -p $(OBJDIR)
	$(CFLAGS) -c $< -o $@

$(OBJDIR)/sigmoidal.o: sigmoidal.cpp
	@mkdir -p $(OBJDIR)
	$(CFLAGS) -c $< -o $@

$(OBJDIR)/perceptron.o: perceptron.cpp
	@mkdir -p $(OBJDIR)
	$(CFLAGS) -c $< -o $@

$(OBJDIR)/training_set.o: training_set.cpp
	@mkdir -p $(OBJDIR)
	$(CFLAGS) -c $< -o $@

$(OBJDIR)/plot.o: plot.cpp
	@mkdir -p $(OBJDIR)
	$(CFLAGS) -c $< -o $@

$(OBJDIR)/util.o: util.cpp
	@mkdir -p $(OBJDIR)
	$(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/*

.PHONY: super_clean
super_clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/* $(PLOTDIR)/*
