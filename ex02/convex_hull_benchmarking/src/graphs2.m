mkdir pics

algNames = {'toussaint', 'bykat', 'eddy', 'graham-andrew', 'jarvis'};

for num = 1:size(algNames,2)
	c = [];
	for i = [0:4]
		inputName = sprintf ('build/output-%d.txt', i);
		data = dlmread (inputName);
		v = data(num+1,:);
		c = [c;v];
	end;
	c

	inon = [];
	for i = [0:4]
		name = sprintf('boundary: %.1f%% interior: %.1f%%', i/4 * 100 , (4-i)/4 * 100);
		inon = [inon;name];
	end;

	imageName = sprintf ('pics/pic-%s.png', algNames{num});

	handle = figure('visible', 'off');
	plot(data(1,:), c, '--s', 'linewidth', 2, 'markersize', 5)
	legend(inon);
	title(sprintf('Convex Hull. Algorithm: %s', algNames{num}));
	xlabel('number of points');
	ylabel('average time (ms)');
	print(imageName);
	close(handle);
end;
