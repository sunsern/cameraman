% N = zeros(500,500);  %Number falling in each cell
% P0 = zeros(size(N)); %Number of particles at depth 0 in each cell
% P1 = zeros(size(N)); %Number of particles at depth 1 in each cell
% P2 = zeros(size(N)); %Number of particles at depth 2 in each cell
% H = zeros(size(N));
load('/Users/cameraman/cameraman/controller/cameraman_audio_sketch/audio_saved_data.mat');
NUM_READ = 200000;
fid = fopen('/Users/cameraman/cameraman/controller/cameraman_audio_sketch/pd_trees.csv');
while 1    
    X = textscan(fid,'%d,%f,%f,%f,%d,%d,%d',NUM_READ); 
    for i=1:size(X{1},1)
        row = floor(X{2}(i)/2 + 250);
        col = floor(X{3}(i)/2 + 250);
        if(row < 1 || row > 500 || ...
           col < 1 || col > 500)
            continue;
        end
        N(row,col) = N(row,col) + 1;
        P0(row,col) = P0(row,col) + X{5}(i);
        P1(row,col) = P1(row,col) + X{6}(i);
        P2(row,col) = P2(row,col) + X{7}(i); 
        if(P1(row,col) > P2(row,col) && ...
           P1(row,col) > P0(row,col))
            H(row,col) = 1/3;
        elseif(P2(row,col) > P0(row,col))
            H(row,col) = 2/3;
        else
            H(row,col) = 0;
        end           
    end
    if(size(X{1},1) < NUM_READ), break, end
    disp('iter done');
end
fclose(fid);
P0 = P0./(P0+P1+P2+1e-20);
P1 = P1./(P0+P1+P2+1e-20);
P2 = P2./(P0+P1+P2+1e-20);

logN = log(N + ones(size(N)));
maxV = log(max(N(:)));
I_HSV = zeros(500,500,3);
I_HSV(:,:,1) = H;%(1/3)*P1 + (2/3)*P2;
I_HSV(:,:,2) = ones(size(N));
I_HSV(:,:,3) = logN./maxV;
I_RGB = hsv2rgb(I_HSV);
h = fspecial('gaussian',5,5);
I_RGB=imfilter(I_RGB,h);
imwrite(I_RGB,'/Users/cameraman/cameraman/controller/cameraman_audio_sketch/audio_visualization_background.png', 'png');
save('/Users/cameraman/cameraman/controller/cameraman_audio_sketch/audio_saved_data.mat', 'N', 'H', 'P0', 'P1', 'P2');
!rm /Users/cameraman/Documents/pdtree_log.txt
exit
