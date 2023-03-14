CXX=nvcc
CXXFLAGS=-std=c++11 -O3 -Xcompiler -fopenmp
LDFLAGS=-std=c++11 -O3 -Xcompiler -fopenmp

all: wdg

clean:
	$(RM) *.o

wdg: main.o BasinIndexLoader.o BasinIndexMatrix.o FlowDirectionLoader.o FlowDirectionMatrix.o FlowDirectionReverser.o IWatershedDelineationAlgorithm.o LabelPullingGpuAlgorithm.o PathReductionBackBufferGpuAlgorithm.o PathReductionGpuAlgorithm.o PathTracingGpuAlgorithm.o RecursiveSequentialAlgorithm.o WatershedDelineationUtilities.o
	$(CXX) $(LDFLAGS) -o $@ $^ `gdal-config --libs`

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< `gdal-config --cflags`

%.o: %.cu
	$(CXX) $(CXXFLAGS) -c $< `gdal-config --cflags`
