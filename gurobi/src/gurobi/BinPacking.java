package gurobi;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.io.StreamTokenizer;

import gurobi.GRB;
import gurobi.GRBEnv;
import gurobi.GRBException;
import gurobi.GRBLinExpr;
import gurobi.GRBModel;
import gurobi.GRBQuadExpr;
import gurobi.GRBVar;

public class BinPacking {

	public static GRBEnv env;
	public static GRBModel model;

	public BinPacking(String filename) throws IOException {
	}

	public static void main(String[] args) throws IOException, GRBException {
		/*
		 * instance0,65,66
		 * 1,65,66
		 * 2,132,133
		 * 3,132,133
		 * 4,198,199
		 */

		Reader fileInst = new BufferedReader(new FileReader("../bpp_instances/instance5.bpp"));
		StreamTokenizer stok = new StreamTokenizer(fileInst);

		stok.nextToken();
		int n = (int) stok.nval;
		stok.nextToken();
		int c = (int) stok.nval;
		
		int[] w = new int[n];
		for (int i = 0; i < n; i++)
		{
			stok.nextToken();
			w[i] = (int) stok.nval;
			
			System.out.println(w[i]);
		}
		
		env = new GRBEnv("mip1.log");
		model = new GRBModel(env);
		
		// execution time in seconds 
		model.getEnv().set(GRB.DoubleParam.TimeLimit, 10 * 60.0);

		
		GRBVar[] y = new GRBVar[n];
		GRBVar[][] x = new GRBVar[n][n];

		for (int i = 0; i < n; i++) {
			y[i] = model.addVar(0, 1, 0.0f, GRB.BINARY, "y[" + i + "]");
		}

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				x[i][j] = model.addVar(0, 1, 0.0f, GRB.BINARY, "x[" + i + "][" + j + "]");
			}
		}

		model.update();

		// objective functions
		GRBLinExpr obj = new GRBLinExpr();
		for (int i = 0; i < n; i++) {
			obj.addTerm(1.0, y[i]);
		}

		model.setObjective(obj);
		model.set(GRB.IntAttr.ModelSense, 1);

		for (int i = 0; i < n; i++) {
			GRBLinExpr lhs = new GRBLinExpr();
			for (int j = 0; j < n; j++) {
				lhs.addTerm(w[j], x[i][j]);
			}
			
			GRBLinExpr rhs = new GRBLinExpr();
			rhs.addTerm(c, y[i]);
			
			model.addConstr(lhs, GRB.LESS_EQUAL, rhs, "");
		}

		for (int j = 0; j < n; j++) {
			GRBLinExpr lhs = new GRBLinExpr();
			for (int i = 0; i < n; i++) {
				lhs.addTerm(1.0, x[i][j]);
			}
			
			model.addConstr(lhs, GRB.EQUAL, 1.0, "");
		}

		for (int i = 0; i + 1 < n; i++) {
			GRBLinExpr lhs = new GRBLinExpr();
			lhs.addTerm(1.0, y[i]);

			GRBLinExpr rhs = new GRBLinExpr();
			rhs.addTerm(1.0, y[i + 1]);
			
			model.addConstr(lhs, GRB.GREATER_EQUAL, rhs, "");
		}

		model.update();
		
		// write model to file
		model.write("model.lp");

		model.optimize();

		System.out.println("\n\nZ* = " + model.get(GRB.DoubleAttr.ObjVal));
		
		model.dispose();
		env.dispose();

	}
}
