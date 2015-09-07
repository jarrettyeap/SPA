#include <RelationEvaluator.h>
#include <sstream>

RelationEvaluator::RelationEvaluator(PKB* pkb) {
    this->pkb = pkb;
}
int RelationEvaluator::getInteger(QNode* node) {
    std::istringstream iss(node->getString());
    int result;
    iss >> result;
    return result;
}

bool RelationEvaluator::isSynonym(QNodeType type) {
    return type == WHILESYNONYM ||
           type == ASSIGNSYNONYM ||
           type == VARIABLESYNONYM ||
           type == PROCEDURESYNONYM ||
           type == CONSTSYNONYM ||
           type == PROGLINESYNONYM ||
           type == IFSYNONYM ||
           type == STMTSYNONYM;
}

TType RelationEvaluator::synonymToTType(QNodeType type) {
    if (type == WHILESYNONYM) {
        return WHILEN;
    } else if (type == ASSIGNSYNONYM) {
        return ASSIGNN;
    } else if (type == VARIABLESYNONYM) {
        return VARN;
    } else if (type == PROCEDURESYNONYM) {
        return PROCEDUREN;
    } else if (type == STMTSYNONYM) {
        return STMTN;
    } else if (type == PROGLINESYNONYM) {
	    return STMTN;
    }
}

vector<STMTLINE> RelationEvaluator::filter(vector<STMTLINE> original, TType type) {
    if (type == STMTN) {
        return original;
    }
    vector<STMTLINE> result;
    for (vector<STMTLINE>::size_type i = 0; i < original.size(); i++) {
        if (pkb->getAst()->getTNode(original[i])->getTType() == type) {
            result.push_back(original[i]);
        }
    }
    return result;
}