#include <iostream>
#include <pcl/console/parse.h>
#include <pcl/console/print.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/io/pcd_io.h>
#include <pcl/keypoints/harris_3d.h>
#include <pcl/visualization/pcl_visualizer.h>

int
main(int argc, char** argv)
{
	if (argc < 2)
	{
		pcl::console::print_info("Keypoints indices example application.\n");
		pcl::console::print_info("Syntax is: %s <source-pcd-file>\n", argv[0]);
		return (1);
	}

	pcl::console::print_info("Reading %s\n", argv[1]);

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	if (pcl::io::loadPCDFile<pcl::PointXYZRGB>(argv[1], *cloud) == -1) // load the file
	{
		pcl::console::print_error("Couldn't read file %s!\n", argv[1]);
		return (-1);
	}

	pcl::HarrisKeypoint3D <pcl::PointXYZRGB, pcl::PointXYZI> detector;
	pcl::PointCloud<pcl::PointXYZI>::Ptr keypoints8(new pcl::PointCloud<pcl::PointXYZI>);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr keypointsrgb(new pcl::PointCloud<pcl::PointXYZRGB>);

	detector.setNonMaxSupression(true);
	detector.setInputCloud(cloud);
	//detector.setThreshold (0.000000002);
	detector.setRadius(0.001f);
	detector.setRadiusSearch(0.0045f);
	pcl::StopWatch watch;
	detector.compute(*keypoints8);

	pcl::PointIndicesConstPtr keypoints2_indices = detector.getKeypointsIndices();

	pcl::copyPointCloud(*cloud, *keypoints2_indices, *keypointsrgb); // 

	pcl::io::savePCDFile("keypointsrgb.pcd", *keypointsrgb, true)

	pcl::visualization::PCLVisualizer viewer("ISS 3D");
	viewer.addPointCloud(cloud, "scena");
	viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "scena");


	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZRGB> keypoints_color_handler(keypointsrgb, 0, 0, 255);
	viewer.addPointCloud(keypointsrgb, keypoints_color_handler, "scene_keypoints");
	viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 4, "scene_keypoints");

	while (!viewer.wasStopped())
	{
		viewer.spinOnce();
	}

}